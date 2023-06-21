// DONE: add << >> support (infile, outfile) - test more fully
// TODO: remove fprintf's (replace with writes?)
// TODO: fix issue that minishell exits before allowing next line to be captured (it's fine with a single command)

// - when we do ls | ls, output of ls is treated as input to minishell because it goes to std_out
// Q: if a command is not found, do we stop there or do we continue piping? A: we continue piping but next command receives no input

// approach: check a valid path for the command is found in env, and ONLY THEN create the pipe and execute the command in the child process
// special case for the last command or single command, which will be executed without piping (as there is no interprocess communication in this case)
// when running as minishell, the LAST COMMAND should be run as a subprocess so as not to take over the main minishell process, but we do not need its piped output(so no pipe)
// whereas on the tester we just return to the command line

#include "minishell.h"

void	print_array(char **array)
{
	int i;

	i = 0;
	while (array[i])
	{
		fprintf(stderr, "%s ", array[i]);
		i++;
	}
	fprintf(stderr, "\n");
}

// convert linked list of cmd arguments to an array (which execve expects)
char **list_to_array(t_list *list_head)
{
	int l;
	char **array;
	int i;
	t_list *list_head_cpy;

	list_head_cpy = list_head;
	l = 0;
	while(list_head)
	{
		list_head = list_head->next;
		l++;
	}
	array = malloc(sizeof(char *) * (l + 1));
	i = 0;
	while (i < l)
	{
		array[i] = list_head_cpy->content;
		list_head_cpy = list_head_cpy->next;
		i++;
	}
	return (array);
}

char	*get_path(char **cmd_as_array, char **env)
{
	char	**paths;
	int		i;
	char	*path_cmd;

	while (ft_strncmp("PATH=", *env, 5))
		env++;
	paths = ft_split(*env + 5, ':'); // don't need to check if paths doesn't return

	i = 0;
	while (paths[i])
	{
		path_cmd = ft_strjoin(ft_strjoin(paths[i], "/"), cmd_as_array[0]);
		if (access(path_cmd, F_OK | X_OK) == 0) // if file exists and is executable
		{
			free(paths);
			return (path_cmd);
		}
		free(path_cmd);
		i++;
	}
	return (NULL);
}

int	make_heredoc(char *limiter)
{
	int		heredoc_fd;
	char	*next_line;

	fprintf(stderr, "lmt:%s\n", limiter);
	heredoc_fd = open(".heredoc_tmp", O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (heredoc_fd < 0)
		fprintf(stderr, "%s", ERR_HEREDOC);
	while (1)
	{
		write(STDOUT_FD, "pipe heredoc> ", 14);
		next_line = get_next_line(STDIN_FD);
		next_line[ft_strlen(next_line) - 1] = '\0';
		if (ft_strncmp(next_line, limiter, ft_strlen(limiter)) == 0)
			break ;
		write(heredoc_fd, next_line, ft_strlen(next_line));
		write(heredoc_fd, "\n", 1);
		free(next_line);
	}
	free(next_line);
	close(heredoc_fd);
	heredoc_fd = open(".heredoc_tmp", O_RDONLY);
	return (heredoc_fd);
}

int	open_and_redirect_from_infile(t_node *cmd)
{
	int in_fd;

	if(!cmd->infile)
		return (STDIN_FD);
	if (cmd->read_from_heredoc)
		in_fd = make_heredoc(cmd->limiter);
	else
		in_fd = open(cmd->infile, O_RDONLY);
	if (in_fd != -1)
		dup2(in_fd, STDIN_FD);
	return (in_fd);
}

int	open_and_redirect_to_outfile(t_node *cmd)
{
	int out_fd;

	if(!cmd->outfile)
		return (STDOUT_FD);
	if (cmd->append_when_writing == true)
		out_fd = open(cmd->outfile, O_CREAT | O_WRONLY | O_APPEND, 0644);
	else
		out_fd = open(cmd->outfile, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (out_fd != -1)
		dup2(out_fd, STDOUT_FD);
	return (out_fd);
}

void execute_cmd(t_list *command_elements, char **env)
{
	char **cmd_as_array;
	char *path;

	cmd_as_array = list_to_array(command_elements);
	if (check_for_builtin(cmd_as_array[0]) == true)
	{
		run_builtin(cmd_as_array, env);
		return ;
	}
	path = get_path(cmd_as_array, env);
	if (path == NULL)
	{
		fprintf(stderr, "%s%s", cmd_as_array[0], ERR_CMD);
		return ;
	}
	//cmd_as_array[0] = path;
	// print_array(cmd_as_array);
	if (execve(path, cmd_as_array, env) == -1)
	{
		fprintf(stderr, "%s", ERR_EXEC);
		free(cmd_as_array);
		exit(1); // 0 = success, non-zero = different types of failures
	}
}


// when is_last_command == true we skip the pipe create and just go to standard out
void	pipe_to_parent(t_node *cmd_node, char **env, bool is_last_command)
{
	pid_t	pid;
	int		io_fd[2];
	int		exit_status;
	int		in_fd;
	int		out_fd;

	in_fd = open_and_redirect_from_infile(cmd_node);
	if (in_fd == -1)
	{
		fprintf(stderr, "bash: %s%s", cmd_node->infile, ERR_READ);
		return ;
	}
	out_fd = open_and_redirect_to_outfile(cmd_node);
	if (out_fd == -1) // TODO: how can this error occur, files not found are made, file that are found are appended
	{
		fprintf(stderr, "bash: %s%s", cmd_node->outfile, ERR_WRITE);
		return ;
	}			
	if (!is_last_command)
		pipe(io_fd); // TODO: pipe errors
	pid = fork();
	if (pid == -1)
	{
		fprintf(stderr, "%s", ERR_FORK);
		exit(1);
	}
	if (pid == 0)
	{
		if (!is_last_command)
		{
			close(io_fd[0]);
			dup2(io_fd[1], STDOUT_FD);
		}
		execute_cmd(cmd_node->command_elements, env);
	}
	else
	{
		if (!is_last_command)
		{
			close(io_fd[1]);
			dup2(io_fd[0], STDIN_FD);
		}
		wait(&exit_status);
		if (exit_status != 0)
			//fprintf(stderr, "child process exited abnormally with status %i\n", exit_status);
		if (in_fd != STDIN_FD)
			close(in_fd);
		if (out_fd != STDOUT_FD)
			close(out_fd);
		if (cmd_node->read_from_heredoc == true)
			unlink(".heredoc_tmp");		
	}
}

void	traverse_ast2(t_node *head, t_node *current, char **env)
{
	if (current->type == N_CMD)
	{
		if (current == head) // at last command: execute cmd to standard out replacing current process (no pipe)
			pipe_to_parent(current, env, true);
		else // not at last command: execute it in a subprocess, but to a pipe
			pipe_to_parent(current, env, false);
	}
	else
	{
		traverse_ast2(head, current->left, env);
		if (current->type == N_PIPE)
		{
			if (current == head) // at last pipe: execute right branch cmd in a subprocess, but not to a pipe
				pipe_to_parent(current->right, env, true);
			else // before last pipe: execute right branch cmd in a subprocess, but to a pipe
				pipe_to_parent(current->right, env, false);
		}
	}
}

void	traverse_ast(t_node **root, char **env)
{
	t_node *head;

	head = *root;
	traverse_ast2(head, *root, env);
	// free(head); // why is this a double free?
}
