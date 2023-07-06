#include "minishell.h"

int	dollar_spotted(t_token *t, char **env, int c)
{
	char	*before;
	char	*searched;
	char	*after;

	before = make_before(t, c);
	searched = make_searched(t, &c);
	if (searched[0] == '\0')
		return (c);
	after = make_after(t, c);
	c = match_searched(env, searched);
	if (c == -1)
		return (not_found(t, before, after, searched));
	else
	{
		free(t->content);
		t->content = searched;
		if (c == -2)
			searched = int_to_string(g_exitstatus);
		else
			searched = env[c] + ft_strlen(searched);
		found(t, before, after, searched);
		if (c == -2)
			free(searched);
		return (0);
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

int	not_found(t_token *t, char *before, char *after, char *searched)
{
	free(searched);
	searched = t->content;
	t->content = ft_strjoin(before, after);
	free(searched);
	free(before);
	free(after);
	return (-1);
}

int	match_searched(char **env, char *searched)
{
	int	c;

	c = 0;
	if (ft_strncmp(searched, "?=", 3) == 0)
		return (-2);
	while (env[c] != NULL)
	{
		if (ft_strncmp(env[c], searched, ft_strlen(searched)) == 0)
			return (c);
		c++;
	}
	return (-1);
}

void	strip_quotes(t_token *t, char a)
{
	int		c;
	char	*replacer;

	replacer = ft_calloc(ft_strlen(t->content) - 1, sizeof(char));
	c = 0;
	while (t->content[c + 1] != a)
	{
		replacer[c] = t->content[c + 1];
		c++;
	}
	free(t->content);
	t->content = replacer;
}
