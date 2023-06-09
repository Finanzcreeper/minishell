/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interpreter_1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nreher <nreher@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/06 14:43:18 by gbooth            #+#    #+#             */
/*   Updated: 2023/07/07 14:06:13 by nreher           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_exitstatus;

void	execute_cmd(char **cmdarr, char **env)
{
	char	*path;

	if (*cmdarr == NULL)
	{
		g_exitstatus = 1;
		exit(g_exitstatus);
	}
	path = get_path(cmdarr, env);
	if (path == NULL)
	{
		write(STDERR_FD, cmdarr[0], ft_strlen(cmdarr[0]));
		write(STDERR_FD, ERR_CMD, ft_strlen(ERR_CMD));
		g_exitstatus = 1;
		exit(g_exitstatus);
	}
	if (execve(path, cmdarr, env) == -1)
	{
		write(STDERR_FD, ERR_EXEC, ft_strlen(ERR_EXEC));
		g_exitstatus = 1;
		exit(g_exitstatus);
	}
}

void	child(t_node *cmd_node, char **env, bool lstcmd, int io_fd[2])
{
	if (!lstcmd)
	{
		close(io_fd[0]);
		dup2(io_fd[1], STDOUT_FD);
		close(io_fd[1]);
	}
	run_builtin(cmd_node->cmdarr, env);
	execute_cmd(cmd_node->cmdarr, env);
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
		write(STDERR_FD, ERR_FORK, ft_strlen(ERR_FORK));
		exit(127);
	}
	if (pid == 0)
		child(cmd_node, env, lstcmd, io_fd);
	signal(SIGINT, SIG_IGN);
	wait(&g_exitstatus);
	signal(SIGINT, sigint_handler);
	if (!lstcmd)
	{
		close(io_fd[1]);
		dup2(io_fd[0], STDIN_FD);
		close(io_fd[0]);
	}
	close_inout_fds(cmd_node);
}

void	pipe_to_parent(t_node *cmd_node, char ***env, bool lstcmd)
{
	int	fuckoff;

	cmd_node->cmdarr = list_to_array(cmd_node->command_elements);
	if (cmd_node->read_from_heredoc == true)
		make_heredoc(cmd_node);
	if (*(cmd_node->cmdarr) != NULL)
	{
		fuckoff = 1;
		fuckoff = fuck_this(cmd_node, env);
		if (fuckoff == 0)
			return ;
	}
	open_infile(cmd_node);
	open_outfile(cmd_node);
	if (cmd_node->in_fd == -1 || cmd_node->out_fd == -1)
		return ;
	forker(cmd_node, *env, lstcmd);
}

void	traverse_ast(t_node *ast, char ***env)
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
