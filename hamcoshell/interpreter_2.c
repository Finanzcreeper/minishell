/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interpreter_2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nreher <nreher@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/06 14:43:34 by gbooth            #+#    #+#             */
/*   Updated: 2023/07/07 13:59:16 by nreher           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_paths(char	**paths)
{
	int		i;

	i = 0;
	while (paths[i] != NULL)
		free(paths[i++]);
	free(paths);
}

char	*get_path(char **cmdarr, char **env)
{
	char	**paths;
	int		i;
	char	*path_cmd;

	if (access(cmdarr[0], F_OK | X_OK) == 0)
		return (cmdarr[0]);
	while (*env != NULL && ft_strncmp("PATH=", *env, 5))
		env++;
	if (*env != NULL)
	{
		paths = ft_split(*env + 5, ':');
		i = 0;
		while (paths[i])
		{
			fuckynette(&path_cmd, &i, cmdarr, paths);
			if (access(path_cmd, F_OK | X_OK) == 0)
			{
				free_paths(paths);
				return (path_cmd);
			}
			free(path_cmd);
		}
		free_paths(paths);
	}
	return (NULL);
}

void	open_infile(t_node *cmd_node)
{
	cmd_node->in_fd = STDIN_FD;
	if (cmd_node->infile != NULL)
		cmd_node->in_fd = open(cmd_node->infile, O_RDONLY);
	if (cmd_node->in_fd == -1)
	{
		write(STDERR_FD, PRG_NAME, ft_strlen(PRG_NAME));
		write(STDERR_FD, cmd_node->infile, ft_strlen(cmd_node->infile));
		write(STDERR_FD, ERR_FILE, ft_strlen(ERR_FILE));
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
		cmd_node->out_fd = open(cmd_node->outfile,
				O_CREAT | O_WRONLY | O_APPEND, 0644);
	else
		cmd_node->out_fd = open(cmd_node->outfile,
				O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (cmd_node->out_fd == -1)
	{
		write(STDERR_FD, PRG_NAME, ft_strlen(PRG_NAME));
		write(STDERR_FD, cmd_node->outfile, ft_strlen(cmd_node->outfile));
		write(STDERR_FD, ERR_WRITE, ft_strlen(ERR_WRITE));
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
	if (cmd_node->read_from_heredoc == true)
		unlink(".heredoc_tmp");
}
