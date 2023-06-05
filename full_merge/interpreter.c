// TODO: fix hangs with single commands
// TODO: add << >> support (infile, outfile)
// TODO: add builtins
// Q: if a command is not found, do we stop there or do we continue piping?

// approach: check a valid path for the command is found in env, and ONLY THEN create the pipe and execute the command in the child process
// special case for the last command or single command, which will be executed without piping (as there is no interprocess communication in this case)

// EXAMPLE AST:
//		 		P0
// 			/		\
//	 		P1		C2
// 		/		\	| 
// 		P3		C4	head -n 2
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

void	pipe_to_parent(t_list *command_elements, char **env)
{
	pid_t	pid;
	int		io_fd[2];
	int		exit_status;

	pipe(io_fd); // TODO: pipe errors
	pid = fork();
	if (pid == -1)
	{
		fprintf(stderr, "%s\n", ERR_FORK);
		exit(1);
	}
	if (pid == 0)
	{
		// printf("child!\n");
		close(io_fd[0]);
		dup2(io_fd[1], STDOUT_FD);
		execute_cmd(command_elements, env);
	}
	else
	{
		// printf("parent!\n");
		close(io_fd[1]);
		dup2(io_fd[0], STDIN_FD);
		waitpid(pid, &exit_status, 0);
		// if (exit_status == 0)
		// 	fprintf(stderr, "child process exited successfully\n");
		// else
		// 	fprintf(stderr, "child process exited abnormally with status %i\n", exit_status);
	}
}

void	traverse_ast2(t_node *head, t_node *current, char **env)
{
	if (current->type == N_CMD)
	{
		if (current == head)
		{
			//printf("found cmd, executing in parent process\n");
			printf("[CP]\n");
			execute_cmd(current->command_elements, env);
		}
		else
		{
			//printf("found cmd, executing in subprocess\n");
			printf("[CS]\n");
			pipe_to_parent(current->command_elements, env);
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
				// printf("found pipe: executing right branch cmd in parent process\n");
				printf("[PP]\n");
				execute_cmd(current->right->command_elements, env);
			}
			else
			{
				//printf("found pipe: executing right branch cmd in subprocess\n");
				printf("[PS]\n");
				pipe_to_parent(current->right->command_elements, env);
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
