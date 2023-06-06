// TODO: fix hangs with single commands
// TODO: add << >> support (infile, outfile)
// TODO: add builtins
// Q: if a command is not found, do we stop there or do we continue piping?

// approach: check a valid path for the command is found in env, and ONLY THEN create the pipe and execute the command in the child process
// special case for the last command or single command, which will be executed without piping (as there is no interprocess communication in this case)

// when running as minishell, the LAST COMMAND should be run as a subprocess so as not to take over the main minishell process, but we do not need its piped output(so no pipe)
// whereas on the tester we just return to the command line

// EXAMPLE AST:
//		 		P0
// 			/		\
//	 		P1		C2
// 		/		\	| 
// 		P3		C4	wc -l
// 	/		\	|
// C5		C6	sort
// |		|
// ls 	grep micro

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

void execute_cmd(t_list *command_elements, char **env)
{
	char **cmd_as_array;

	cmd_as_array = list_to_array(command_elements);
	if (check_for_builtin(cmd_as_array[0]) == true)
	{
		run_builtin(cmd_as_array, env);
		return ;
	}

	cmd_as_array[0] = get_path(cmd_as_array, env);
	if (cmd_as_array == NULL)
	{
		fprintf(stderr, "%s\n", ERR_CMD);
		return ;
	}
	print_array(cmd_as_array);
	if (execve(cmd_as_array[0], cmd_as_array, env) == -1)
	{
		fprintf(stderr, "%s\n", ERR_EXEC);
		free(cmd_as_array);
		exit(1); // 0 = success, non-zero = different types of failures
	}
}

// when is_last_command == true we skip the pipe create and just go to standard out
void	pipe_to_parent(t_list *command_elements, char **env, bool is_last_command)
{
	pid_t	pid;
	int		io_fd[2];
	int		exit_status;

	if (!is_last_command)
		pipe(io_fd); // TODO: pipe errors
	pid = fork();
	// fprintf(stderr, "1: %i\n", pid);
	if (pid == -1)
	{
		fprintf(stderr, "%s\n", ERR_FORK);
		exit(1);
	}
	if (pid == 0)
	{
		// printf("child!\n");
		if (!is_last_command)
		{
			close(io_fd[0]);
			dup2(io_fd[1], STDOUT_FD);
		}
		execute_cmd(command_elements, env);
	}
	else
	{
		printf("parent!\n");
		if (!is_last_command)
		{
			close(io_fd[1]);
			dup2(io_fd[0], STDIN_FD);
		}
		printf("blocking parent!\n");
		fprintf(stderr, "2: %i\n", pid);
		wait(&exit_status); // this blocks forever!
		printf("parent unblocked!\n");
		if (exit_status == 0)
			fprintf(stderr, "child process exited successfully\n");
		else
			fprintf(stderr, "child process exited abnormally with status %i\n", exit_status);
	}
}

void	traverse_ast2(t_node *head, t_node *current, char **env)
{
	if (current->type == N_CMD)
	{
		if (current == head)
		{
			// at last command: execute cmd to standard out replacing current process (no pipe)
			pipe_to_parent(current->command_elements, env, true);
			// do this when in non-interactive mode
			// execute_cmd(current->command_elements, env, true);
		}
		else
		{
			// not at last command: execute it in a subprocess, but to a pipe
			pipe_to_parent(current->command_elements, env, false);
		}
	}
	else
	{
		//printf("going down\n");
		traverse_ast2(head, current->left, env);
		//printf("coming back up\n");
		if (current->type == N_PIPE)
		{
			if (current == head)
			{
				// at last pipe: execute right branch cmd in a subprocess, but not to a pipe
				pipe_to_parent(current->right->command_elements, env, true);
			}
			else
			{
				// before last pipe: execute right branch cmd in a subprocess, but to a pipe
				pipe_to_parent(current->right->command_elements, env, false);
			}
		}
	}
}

void	traverse_ast(t_node *root, char **env)
{
	t_node *head;

	head = ft_calloc(1, sizeof(t_node));
	head = root;
	traverse_ast2(head, root, env);
	// free(head); // why is this a double free?
}
