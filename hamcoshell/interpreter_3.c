/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interpreter_3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nreher <nreher@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/06 14:43:40 by gbooth            #+#    #+#             */
/*   Updated: 2023/07/07 14:06:15 by nreher           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

void	make_heredoc(t_node *cmd_node)
{
	int		heredoc_fd;
	char	*next_line;

	heredoc_fd = open(".heredoc_tmp", O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (heredoc_fd < 0)
		write(STDERR_FD, ERR_HEREDOC, ft_strlen(ERR_HEREDOC));
	while (1)
	{
		next_line = readline("pipe heredoc> ");
		if (next_line == NULL || 
			ft_strncmp(next_line, cmd_node->limiter,
				ll(next_line, cmd_node->limiter)) == 0)
			break ;
		write(heredoc_fd, next_line, ft_strlen(next_line));
		write(heredoc_fd, "\n", 1);
		free(next_line);
	}
	cmd_node->infile = ft_strdup(".heredoc_tmp");
	free(next_line);
	close(heredoc_fd);
}

void	fuckynette(char **path_cmd, int *i, char **cmdarr, char **paths)
{
	char	*pre;

	pre = ft_strjoin(paths[*i], "/");
	*path_cmd = ft_strjoin(pre, cmdarr[0]);
	free(pre);
	*i = *i + 1;
}

int	fuck_this(t_node *cmd_node, char ***env)
{
	if (is_builtin(cmd_node->cmdarr[0], "exit"))
	{
		builtin_exit(cmd_node->cmdarr);
		return (0);
	}
	if (is_builtin(cmd_node->cmdarr[0], "cd"))
	{
		*env = builtin_cd(cmd_node->cmdarr, *env);
		return (0);
	}
	if (is_builtin(cmd_node->cmdarr[0], "export"))
	{
		g_exitstatus = 1;
		*env = builtin_export_args(cmd_node->cmdarr, *env);
		if (g_exitstatus != 1)
			return (0);
	}
	if (is_builtin(cmd_node->cmdarr[0], "unset"))
	{
		*env = builtin_unset(cmd_node->cmdarr, *env);
		return (0);
	}
	return (1);
}
