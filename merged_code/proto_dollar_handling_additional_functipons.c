#include "proto.h"

void	dollar_spotted(t_token *t, char **env, int c)
{
	char	*before;
	char	*searched;
	char	*after;

	before = make_before(t, c);
	searched = make_searched(t, &c);
	after = make_after(t, c);
	c = match_searched(env, searched);
	if (c == -1)
		not_found(t, before, after, searched);
	else
	{
		free(t->content);
		t->content = searched;
		searched = env[c] + ft_strlen(searched);
		found(t, before, after, searched);
	}
}

void	found(t_token *t, char *before, char *after, char *searched)
{
	free(t->content);
	t->content = ft_strjoin(before, searched);
	free(before);
	before = t->content;
	t->content = ft_strjoin(t->content, after);
	free(before);
	free(after);
}

void	not_found(t_token *t, char *before, char *after, char *searched)
{
	free(searched);
	searched = t->content;
	t->content = ft_strjoin(before, after);
	free(searched);
	free(before);
	free(after);
}

int	match_searched(char **env, char *searched)
{
	int	c;

	c = 0;
	while (env[c] != NULL)
	{
		if (ft_strncmp(env[c], searched, ft_strlen(searched)) == 0)
			return (c);
		c++;
	}
	return (-1);
}
