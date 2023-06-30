/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_additional_functions.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nreher <nreher@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/03 10:38:35 by nreher            #+#    #+#             */
/*   Updated: 2023/06/27 15:27:54 by nreher           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

int	g_exitstatus;

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

void	make_heredoc(t_node *cmd_node, char *limiter)
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

void	open_infile(t_node *cmd_node)
{
	cmd_node->in_fd = STDIN_FD;
	if (cmd_node->read_from_heredoc == false)
	{
		if (cmd_node->infile != NULL)
			cmd_node->in_fd = open(cmd_node->infile, O_RDONLY);
	}
	else
	{
		if (cmd_node->read_from_heredoc == true)
			make_heredoc(cmd_node, cmd_node->limiter);
	}
	if (cmd_node->in_fd == -1)
	{
		fprintf(stderr, "bash: %s%s", cmd_node->infile, ERR_READ);
	}
	if (cmd_node->in_fd != STDIN_FD)
	{
		dup2(cmd_node->in_fd, STDIN_FD);
		close(cmd_node->in_fd);
	}
}

void	open_outfile(t_node *cmd_node)
{
	cmd_node->out_fd = STDOUT_FD;
	if (cmd_node->outfile == NULL)
		return ;
	if (cmd_node->append_when_writing == true)
		open(cmd_node->outfile, O_CREAT | O_WRONLY | O_APPEND, 0644);
	else
		open(cmd_node->outfile, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (cmd_node->out_fd == -1)
	{
		fprintf(stderr, "bash: %s%s", cmd_node->outfile, ERR_WRITE);
	}
	if (cmd_node->out_fd != STDOUT_FD)
	{
		dup2(cmd_node->out_fd, STDOUT_FD);
		close(cmd_node->out_fd);
	}
}

void	close_inout_fds(t_node *cmd_node)
{
	if (g_exitstatus != 0 && cmd_node->in_fd != STDIN_FD)
		close(cmd_node->in_fd);
	if (cmd_node->out_fd != STDOUT_FD)
		close(cmd_node->out_fd);
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
		g_exitstatus = 1;
		exit(g_exitstatus);
	}
	if (execve(path, cmd_as_array, env) == -1)
	{
		fprintf(stderr, "%s", ERR_EXEC);
		free(cmd_as_array);
		g_exitstatus = 1;
		exit(g_exitstatus);
	}
}

void	child(t_node *cmd_node, char **env, bool lstcmd, int io_fd[2])
{
	char	**cmd_as_array;

	if (!lstcmd)
	{
		close(io_fd[0]);
		dup2(io_fd[1], STDOUT_FD);
		close(io_fd[1]);
	}
	cmd_as_array = list_to_array(cmd_node->command_elements);
	if (check_for_builtin(cmd_as_array[0]) == true)
	{
		run_builtin(cmd_as_array, env);
		free(cmd_as_array);
		exit(g_exitstatus);
	}
	else
	{
		free(cmd_as_array);
		execute_cmd(cmd_node->command_elements, env);
	}
}

void	forker(t_node *cmd_node, char **env, bool lstcmd)
{
	int		io_fd[2];
	pid_t	pid;

	if (!lstcmd)
		pipe(io_fd);
	pid = fork();
	if (pid == -1)
	{
		fprintf(stderr, "%s", ERR_FORK);
		exit(127);
	}
	if (pid == 0)
		child(cmd_node, env, lstcmd, io_fd);
	else
	{
		wait(&g_exitstatus);
		if (!lstcmd)
		{
			close(io_fd[1]);
			dup2(io_fd[0], STDIN_FD);
			close(io_fd[0]);
		}
		close_inout_fds(cmd_node);
		if (cmd_node->read_from_heredoc == true)
			unlink(".heredoc_tmp");
	}
}

void	pipe_to_parent(t_node *cmd_node, char **env, bool lstcmd)
{
	char	**cmd_as_array;

	cmd_as_array = list_to_array(cmd_node->command_elements);
	if (ft_strncmp(cmd_as_array[0], "exit", ft_strlen(cmd_as_array[0])) == 0)
	{
		builtin_exit(cmd_as_array);
		free(cmd_as_array);
		return ;
	}
	if (ft_strncmp(cmd_as_array[0], "cd", ft_strlen(cmd_as_array[0])) == 0)
	{
		builtin_cd(cmd_as_array, env);
		free(cmd_as_array);
		return ;
	}
	open_infile(cmd_node);
	if (cmd_node->in_fd == -1)
		return ;
	open_outfile(cmd_node);
	if (cmd_node->out_fd == -1)
		return ;
	forker(cmd_node, env, lstcmd);
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
