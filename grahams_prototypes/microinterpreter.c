// manually build an ast of the two forms of command handled in the grammar i.e. "ls" and "ls | wc"
// traverse the tree, executing the commands in subshells
// cc -Wall -Werror -Wextra -ILibft microinterpreter.c  Libft/libft.a -o microinterpreter && ./microinterpreter
// - everytime we encounter a pipe (or the root node) in the tree, we
//	- fork off a child process, setup read end of the pipe
//	- in parent process, setup write end of the pipe
// the root node is treated like a pipe, so that there is a watchover process to exit to
// this way things should work nicely even when there is no pipe in the ast

// Q: why is there no output?!?

#include "micro.h"

// convert linked list of cmd arguments to an array (which execve expects)
char **list_to_array(t_token *list_head)
{
	int l;
	char **array;
	int i;

	t_token *list_head_cpy;

	list_head_cpy = list_head;
	l = 0;
	while(list_head)
	{
		list_head = list_head->next;
		l++;
	}
	array = malloc(sizeof(char *) * (l + 1)); // TODO: how to free this? result is used in execve
	i = 0;
	while (i < l)
	{
		array[i] = list_head_cpy->content;
		// fprintf(stderr, "%s\n", list_head_cpy->content);
		list_head_cpy = list_head_cpy->next;
		i++;
	}
	array[i] = NULL;
	return (array);
}

void	execute_cmd2(char **paths, t_token *cmd, char **env)
{
	int		j;
	char	*path_cmd;
	char	**cmd_as_array;

	j = 0;
	while (paths[j])
	{
		path_cmd = ft_strjoin(ft_strjoin(paths[j++], "/"), cmd->content);
		if (access(path_cmd, 0) == 0)
		{
			free(paths);
			cmd = cmd->next;
			// fprintf(stderr, "%s ", path_cmd);
			cmd_as_array = list_to_array(cmd);
			if (execve(path_cmd, cmd_as_array, env) == -1)
			{
				perror("Execution error");
				exit(127);
			}
			free(cmd_as_array);
		}
		free(path_cmd);
	}
}

void	execute_cmd(t_token *cmd, char **env)
{
	char	**paths;

	while (ft_strncmp("PATH=", *env, 5))
		env++;
	paths = ft_split(*env + 5, ':');
	if (!paths) // what kind of error is this? when there is not environment? what error text should be printed?
		exit(1);
	execute_cmd2(paths, cmd, env);
}

// run the given command in a forked subprocess, returning the output (stdout) to the parent process (stdin)
// pid == 0 is child, pid > 0 is parent, ip pipe goes child [out] -> parent [in]
void	pipe_to_parent(t_token *cmd, char **env)
{
	pid_t	pid;
	int		io_fd[2];

	pipe(io_fd);
	pid = fork();
	if (pid == -1)
		perror(ERR_FORK);
	if (pid == 0)
	{
		close(io_fd[0]);
		dup2(io_fd[1], STDOUT_FD);
		execute_cmd(cmd, env);
		fprintf(stderr, "%s: %s\n", cmd->content, ERR_CMD);
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
	if (node->type == N_PIPE)
	{
		visit_and_execute(node->left, env);
		visit_and_execute(node->right, env);
		free(node);
	}
	if (node->type == N_CMD)
	{
		pipe_to_parent(node->cmd_elements, env);
		free(node);
	}
}
