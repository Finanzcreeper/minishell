// clear; cc -Wall -Werror -Wextra interpreter_newbuild.c libft/ft_lstnew.c libft/ft_lstadd_back.c libft/ft_lstlast.c libft/ft_split.c libft/ft_strjoin.c libft/ft_substr.c libft/ft_strncmp.c libft/ft_strlen.c libft/ft_calloc.c libft/ft_bzero.c && ./a.out

// TODO: fix hangs with single commands
// TODO: add << >> support (infile, outfile)
// TODO: add builtins

// order of operations:
// traverse tree, if P encountered and left branch is C, execute ls in child process, give result back to parent
// now traverse right branch of P, execute wc in child process, give result back to parent
// keep doing this as we go up and done

// new approach: check a valid path for the command is found in env, and ONLY THEN create the pipe and execute the command in the child process
// but still no output!

// we need a special case for the last command or single command, which will be executed without piping (as there is no interprocess communication in this case)

// NEXT: if a command is not found, do we stop there or do we continue piping?

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <string.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/ioctl.h>
# include <stdbool.h>
# include "../libft/libft.h"

# define STDIN_FD 0
# define STDOUT_FD 1
# define STDERR_FD 2

# define ERR_CMD "Command Not Found"
# define ERR_FORK "Fork error!"
# define ERR_EXEC "Execution error!"

typedef enum e_node_types
{
	N_CMD,
	N_PIPE,
}					t_node_types;

typedef struct s_node
{
	int				type;
	struct s_node	*left;
	struct s_node	*right;
	t_list			*cmd_elements;
	char			*infile;
	char			*outfile;
}					t_node;

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

char	**get_path(t_list *cmd_elements, char **env)
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
		path_cmd = ft_strjoin(ft_strjoin(paths[i], "/"), cmd_elements->content);
		if (access(path_cmd, F_OK | X_OK) == 0) // if file exists and is executable
		{
			free(paths);
			cmd_elements->content = path_cmd;
			return (list_to_array(cmd_elements));
		}
		free(path_cmd);
		i++;
	}
	return (NULL);	
}

void execute_cmd(t_list *cmd_elements, char **env)
{
	char **cmd_as_array;

	cmd_as_array = get_path(cmd_elements, env);
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

void	pipe_to_parent(t_list *cmd_elements, char **env)
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
		execute_cmd(cmd_elements, env);
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

// AST:
//		 		P0
// 			/		\
//	 		P1		C2
// 		/		\	| 
// 		P3		C4	head -n 2
// 	/		\	|
// C5		C6	sort
// |		|
// ls 	grep micro

void	traverse_ast2(t_node *head, t_node *current, char **env)
{
	if (current->type == N_CMD)
	{
		if (current == head)
		{
			//printf("found cmd, executing in parent process\n");
			printf("[CP]\n");
			execute_cmd(current->cmd_elements, env);
		}
		else
		{
			//printf("found cmd, executing in subprocess\n");
			printf("[CS]\n");
			pipe_to_parent(current->cmd_elements, env);
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
				execute_cmd(current->right->cmd_elements, env);
			}
			else
			{
				//printf("found pipe: executing right branch cmd in subprocess\n");
				printf("[PS]\n");
				pipe_to_parent(current->right->cmd_elements, env);
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

int main(int argc, char **argv, char **env)
{
	t_node nodes[7];
	t_list *cmd_elements1;
	t_list *cmd_elements2;
	t_list *cmd_elements3;
	t_list *cmd_elements4;

	(void)argc;
	(void)argv;

	cmd_elements1 = NULL;
	cmd_elements2 = NULL;
	cmd_elements3 = NULL;
	cmd_elements4 = NULL;

	ft_lstadd_back(&cmd_elements1, ft_lstnew("ls"));
	ft_lstadd_back(&cmd_elements2, ft_lstnew("grep"));
	ft_lstadd_back(&cmd_elements2, ft_lstnew("micro"));
	ft_lstadd_back(&cmd_elements3, ft_lstnew("sort"));
	ft_lstadd_back(&cmd_elements3, ft_lstnew("-r"));	
	ft_lstadd_back(&cmd_elements4, ft_lstnew("wc"));
	ft_lstadd_back(&cmd_elements4, ft_lstnew("-l"));

	nodes[0].type = N_PIPE;
	nodes[1].type = N_PIPE;
	nodes[2].type = N_CMD;
	nodes[3].type = N_PIPE;
	nodes[4].type = N_CMD;
	nodes[5].type = N_CMD;
	nodes[6].type = N_CMD;

	nodes[0].left = &nodes[1];
	nodes[0].right = &nodes[2];
	nodes[1].left = &nodes[3];
	nodes[1].right = &nodes[4];
	nodes[3].left = &nodes[5];
	nodes[3].right = &nodes[6];		

	nodes[2].left = NULL;
	nodes[2].right = NULL;
	nodes[4].left = NULL;
	nodes[4].right = NULL;
	nodes[5].left = NULL;
	nodes[5].right = NULL;
	nodes[6].left = NULL;
	nodes[6].right = NULL;
			
	nodes[5].cmd_elements = cmd_elements1;
	nodes[6].cmd_elements = cmd_elements2;
	nodes[4].cmd_elements = cmd_elements3;
	nodes[2].cmd_elements = cmd_elements4;

	// traverse_ast(&nodes[5], env); // 1st cmd: ls
	// traverse_ast(&nodes[3], env); // 1st pipe: ls | grep micro
	// traverse_ast(&nodes[1], env); // 1st and 2nd pipe : ls | grep micro | sort -r
	// traverse_ast(&nodes[0], env); // 1st 2nd and 3rd pipe : ls | grep micro | sort -r | head -n 2
	
	// traverse_ast(&nodes[6], env); // second command only HANGS
	// traverse_ast(&nodes[4], env); // third command only HANGS
	traverse_ast(&nodes[2], env); // fourth command only HANGS
}