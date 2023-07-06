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
