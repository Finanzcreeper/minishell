// TODO: remove fprintf's (replace with writes?)
// TODO: fix issue that minishell exits before allowing next line to be captured (it's fine with a single command)

// - when we do ls | ls, output of ls is treated as input to minishell because it goes to std_out
// Q: if a command is not found, do we stop there or do we continue piping? A: we continue piping but next command receives no input

// approach: check a valid path for the command is found in env, and ONLY THEN create the pipe and execute the command in the child process
// special case for the last command or single command, which will be executed without piping (as there is no interprocess communication in this case)
// when running as minishell, the LAST COMMAND should be run as a subprocess so as not to take over the main minishell process, but we do not need its piped output(so no pipe)
// whereas on the tester we just return to the command line

#include "minishell.h"

int exitstatus;

void	print_array(char **array)
{
	int	i;

	i = 0;
	while (array[i])
	{
		fprintf(stderr, "%s ", array[i]);
		i++;
	}
	fprintf(stderr, "\n");
}

char	**list_to_array(t_list *list_head)
{
	int		l;
	char	**array;
	int		i;
	t_list	*list_head_cpy;

	list_head_cpy = list_head;
	l = 0;
	while (list_head)
	{
		list_head = list_head->next;
		l++;
	}
	array = ft_calloc(l + 1, sizeof(char *));
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
	char	*pre;

	if (access(cmd_as_array[0], F_OK | X_OK) == 0)
		return (cmd_as_array[0]);
	while (ft_strncmp("PATH=", *env, 5))
		env++;
	paths = ft_split(*env + 5, ':');
	i = 0;
	while (paths[i])
	{
		pre = ft_strjoin(paths[i], "/");
		path_cmd = ft_strjoin(pre, cmd_as_array[0]);
		free(pre);
		if (access(path_cmd, F_OK | X_OK) == 0)
		{
			i = 0;
			while (paths[i] != NULL)
				free(paths[i++]);
			free(paths);
			return (path_cmd);
		}
		free(path_cmd);
		i++;
	}
	i = 0;
	while (paths[i] != NULL)
		free(paths[i++]);
	free(paths);
	return (NULL);
}

void make_heredoc(t_node *cmd_node, char *limiter)
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
		if (ft_strncmp(next_line, limiter, ft_strlen(next_line)) == 0)
			break ;
		write(heredoc_fd, next_line, ft_strlen(next_line));
		write(heredoc_fd, "\n", 1);
		free(next_line);
	}
	free(next_line);
	close(heredoc_fd);
	cmd_node->infile = ".heredoc_tmp";
}

int	open_infile(t_node *cmd_node)
{
	int		in_fd;

	in_fd = STDIN_FD;
	if (cmd_node->read_from_heredoc == false)
	{
		if (cmd_node->infile != NULL)
			in_fd = open(cmd_node->infile, O_RDONLY);
	}
	else
	{
		if (cmd_node->read_from_heredoc == true)
			make_heredoc(cmd_node, cmd_node->limiter);
	}
	if (in_fd == -1)
	{
		fprintf(stderr, "bash: %s%s", cmd_node->infile, ERR_READ);
		return (-1);
	}
	if (in_fd != STDIN_FD)
	{
		dup2(in_fd, STDIN_FD);
		close(in_fd);
	}
	return (in_fd);
}

int	open_outfile(t_node *cmd_node)
{
	int	out_fd;

	out_fd = STDOUT_FD;
	if (cmd_node->outfile == NULL)
		return (out_fd);
	if (cmd_node->append_when_writing == true)
		out_fd = open(cmd_node->outfile, O_CREAT | O_WRONLY | O_APPEND, 0644);
	else
		out_fd = open(cmd_node->outfile, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (out_fd == -1)
	{
		fprintf(stderr, "bash: %s%s", cmd_node->outfile, ERR_WRITE);
		return (-1);
	}
	if (out_fd != STDOUT_FD)
	{
		dup2(out_fd, STDOUT_FD);
		close(out_fd);
	}
	return (out_fd);
}

void	close_inout_fds(int in_fd, int out_fd)
{
	if (exitstatus != 0 && in_fd != STDIN_FD)
		close(in_fd);
	if (out_fd != STDOUT_FD)
		close(out_fd);
}

void	execute_cmd(t_list *command_elements, char **env)
{
	char	**cmd_as_array;
	char	*path;

	cmd_as_array = list_to_array(command_elements);
	path = get_path(cmd_as_array, env);
	if (path == NULL)
	{
		fprintf(stderr, "%s%s", cmd_as_array[0], ERR_CMD);
		free(cmd_as_array);
		free(path);
		// ft_lstclear(&command_elements, free);
		exitstatus = 1;
		exit(exitstatus);
	}
	if (execve(path, cmd_as_array, env) == -1)
	{
		fprintf(stderr, "%s", ERR_EXEC);
		free(cmd_as_array);
		exitstatus = 1;
		exit(exitstatus);
	}
}

void	pipe_to_parent(t_node *cmd_node, char **env, bool is_last_command)
{
	pid_t	pid;
	int		io_fd[2];
	int		in_fd;
	int		out_fd;
	char	**cmd_as_array;

	cmd_as_array = list_to_array(cmd_node->command_elements);
	if (ft_strncmp(cmd_as_array[0], "exit", ft_strlen(cmd_as_array[0])) == 0)
	{
		builtin_exit(cmd_as_array);
		free(cmd_as_array);
		return ;
	}
	in_fd = open_infile(cmd_node);
	if (in_fd == -1)
		return ;
	out_fd = open_outfile(cmd_node);
	if (out_fd == -1)
		return ;
	if (!is_last_command)
		pipe(io_fd);
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
			close(io_fd[1]);
		}
		// cmd_as_array = list_to_array(cmd_node->command_elements);
		if (check_for_builtin(cmd_as_array[0]) == true)
		{
			run_builtin(cmd_as_array, env);
			free(cmd_as_array);
			exit(exitstatus);
		}
		else
		{
			free(cmd_as_array);
			execute_cmd(cmd_node->command_elements, env);
		}
	}
	else
	{
		wait(&exitstatus);
		free(cmd_as_array);
		if (!is_last_command)
		{
			close(io_fd[1]);
			dup2(io_fd[0], STDIN_FD);
			close(io_fd[0]);
		}
		close_inout_fds(in_fd, out_fd);
		if (cmd_node->read_from_heredoc == true)
			unlink(".heredoc_tmp");
	}
}

void	traverse_ast(t_node *ast, char **env)
{
	if (ast->type == N_CMD)
	{
		if (ast->top_node == true)
			pipe_to_parent(ast, env, true);
		else
			pipe_to_parent(ast, env, false);
	}
	else
	{
		traverse_ast(ast->left, env);
		if (ast->type == N_PIPE)
		{
			if (ast->top_node == true)
				pipe_to_parent(ast->right, env, true);
			else
				pipe_to_parent(ast->right, env, false);
		}
	}
}
