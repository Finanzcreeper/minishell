// microinterpreter
// manually build an ast of the two forms of command handled in the grammar i.e. "ls" and "ls | wc"
// traverse the tree, executing the commands in subshells
// cc -Wall -Werror -Wextra -Ilibft microinterpreter.c libft/ft_strjoin.c libft/ft_split.c libft/ft_substr.c libft/ft_strlen.c libft/ft_strncmp.c -o microinterpreter && ./microinterpreter
// - everytime we encounter a pipe (or the root node) in the tree, we
//	- fork off a child process, setup read end of the pipe
//	- in parent process, setup write end of the pipe
// the root node is treated like a pipe, so that there is a watchover process to exit to
// this way things should work nicely even when there is no pipe in the ast

// Q: why is there no output?!?

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <string.h>
#include "libft.h"

#define STDIN_FD 0
#define STDOUT_FD 1
#define STDERR_FD 2

#define ERR_CMD "Command not found!"
#define ERR_FORK "Fork error!"

#define ROOT -2

// ast types begin from 0 and represent the types of the orange nodes in the diagram
typedef enum e_ast_types
{
	A_CMD_WORD,
	A_PIPE_SEQUENCE,
}	t_ast_types;

typedef struct s_node
{
	int				type;
	char			*content;
	struct s_node	*left;
	struct s_node	*right;
}					t_node;

void	execute_cmd2(char **paths, char **cmd_args, char **env)
{
	int		j;
	char	*path_cmd;

	j = 0;
	while (paths[j])
	{
		path_cmd = ft_strjoin(ft_strjoin(paths[j++], "/"), cmd_args[0]);
		if (access(path_cmd, 0) == 0)
		{
			free(paths);
			// fprintf(stderr, "%s\n", path_cmd);
			if (execve(path_cmd, cmd_args, env) == -1)
			{
				perror("Execution error");
				exit(127);
			}
		}
		free(path_cmd);
	}
}

void	execute_cmd(char *cmd_args_str, char **env)
{
	char	**paths;
	char	**cmd_args;

	while (ft_strncmp("PATH=", *env, 5))
		env++;
	paths = ft_split(*env + 5, ':');
	if (!paths)
	{
		perror(ERR_CMD);
		exit(1);
	}
	cmd_args = ft_split(cmd_args_str, ' ');
	execute_cmd2(paths, cmd_args, env);
}

// run the given command in a forked subprocess, returning the output (stdout) to the parent process (stdin)
// pid == 0 is child, pid > 0 is parent, ip pipe goes child [out] -> parent [in]
void	pipe_to_parent(char *cmd, char **env)
{
	pid_t	pid;
	int		io_fd[2];

	printf("%s\n", cmd);
	pipe(io_fd);
	pid = fork();
	if (pid == -1)
		perror(ERR_FORK);
	if (pid == 0)
	{
		close(io_fd[0]);
		dup2(io_fd[1], STDOUT_FD);
		execute_cmd(cmd, env);
		perror(ERR_CMD);
		exit(127);
	}
	else
	{
		close(io_fd[1]);
		dup2(io_fd[0], STDIN_FD);
		waitpid(pid, NULL, 0);
	}
}

void visit_and_execute(t_node *node, char **env)
{
	if (!node)
		return ;
	visit_and_execute(node->left, env);
	visit_and_execute(node->right, env);

	if (node->type == A_PIPE_SEQUENCE)
	{
		pipe_to_parent(node->left->content, env);
		pipe_to_parent(node->right->content, env);
		// free(node); // cut off this branch so that it isn't executed again i.e. stop browsing the left branch if we encounter another pipe
	}
	if (node->type == A_CMD_WORD)
	{
		pipe_to_parent(node->content, env);
	}
}

// ls
t_node *manually_make_tree1()
{
	t_node *node;

	node = malloc(sizeof(t_node));
	node->type = A_CMD_WORD;
	node->content = "ls";
	return (node);	
}

// // ls | wc
// t_node *manually_make_tree2()
// {
// 	int i;
// 	t_node *nodes[3];

// 	i = 0;
// 	while(i < 3)
// 	{
// 		nodes[i] = malloc(sizeof(t_node));
// 		i++;
// 	}
// 	nodes[0]->type = A_PIPE_SEQUENCE;
// 	nodes[1]->type = A_CMD_WORD;
// 	nodes[1]->content = "ls";
// 	nodes[2]->type = A_CMD_WORD;
// 	nodes[2]->content = "wc";
// 	nodes[0]->left = nodes[1];
// 	nodes[0]->right = nodes[2];
// 	return (nodes[0]);	
// }

int main(int argc, char **argv, char **envp)
{
	t_node *root_node;

	argc--;
	argv++;
	root_node = manually_make_tree1();
	// root_node = manually_make_tree2();
	visit_and_execute(root_node, envp);
	return (0);
}
