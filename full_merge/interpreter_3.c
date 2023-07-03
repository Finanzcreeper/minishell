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

void	make_heredoc(char *limiter)
{
	int		heredoc_fd;
	char	*next_line;

	heredoc_fd = open(".heredoc_tmp", O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (heredoc_fd < 0)
		write(STDERR_FD, ERR_HEREDOC, ft_strlen(ERR_HEREDOC));
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
}
