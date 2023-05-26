#include "proto.h"

void	expand_dollars(t_token **list, char **env)
{
	char	*searchterm;
	t_token	*temp;

	temp = *list;
	if (temp->content == NULL)
		temp = temp->next;
	while (temp != NULL)
	{
		searchterm = ft_calloc(ft_strlen(temp->content) + 1, sizeof(char));
		if (search_dollar(temp, env, searchterm) == 1)
			return ;
		temp = temp->next;
		free(searchterm);
	}
}

int	search_dollar(t_token *t, char **env, char *searchterm)
{
	int	c;

	c = 0;
	if (t->content[c] == '\'')
		return (0);
	while (t->content[c] != '\0')
	{
		if (t->content[c] == '$')
		{
			dollar_found(searchterm, env, t, c);
			return (1);
		}
		c++;
	}
	return (0);
}

void	dollar_found(char *searchterm, char **env, t_token *t, int c)
{
	int		i;
	char	*tempo;
	char	*after;

	t->content[c] = '\0';
	c++;
	i = 0;
	while (t->content[c] != ' ' && t->content[c] != '	'
		&& t->content[c] != '"' && t->content[c] != '\0')
		searchterm[i++] = t->content[c++];
	searchterm[i] = '=';
	after = ft_strdup(t->content + c);
	i = 0;
	while (env[i] != NULL)
	{
		if (ft_strncmp(searchterm, env[i], ft_strlen(searchterm)) == 0)
		{
			tempo = ftn(env, searchterm, t, i);
			fuck_the_norm(tempo, t, after, searchterm);
			return ;
		}
		i++;
	}
	free(searchterm);
	return ;
}

void	fuck_the_norm(char *tempo, t_token *t, char *after, char *searchterm)
{
	free(tempo);
	tempo = t->content;
	t->content = ft_strjoin(tempo, after);
	free(after);
	free(tempo);
	free(searchterm);
}

char	*ftn(char **env, char *searchterm, t_token *t, int i)
{
	char	*tempo;

	tempo = t->content;
	t->content = ft_strjoin(tempo, env[i] + ft_strlen(searchterm));
	return (tempo);
}
