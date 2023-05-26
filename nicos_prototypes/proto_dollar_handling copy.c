#include "proto.h"

char	*make_after(t_token *t, int c)
{
	char	*after;
	int		i;

	i = c;
	while (t->content[c] != '\0')
		c++;
	after = ft_calloc(c - i + 1, sizeof(char));
	c = 0;
	while (t->content[i] != '\0')
		after[c++] = t->content[i++];
	return (after);
}

char	*make_searched(t_token *t, int *c)
{
	char	*searched;
	int		i;
	int		j;

	i = *c + 1;
	while (t->content[i] != ' ' && t->content[i]
		!= '	' && t->content[i] != '\0' && t->content[i] != '$')
		i++;
	searched = ft_calloc(i - *c + 1, sizeof(char));
	(*c)++;
	j = 0;
	while (*c < i)
		searched[j++] = t->content[(*c)++];
	searched[j] = '=';
	return (searched);
}

char	*make_before(t_token *t, int c)
{
	char	*before;
	int		i;

	before = ft_calloc(c + 1, sizeof(char));
	i = 0;
	while (i < c)
	{
		before[i] = t->content[i];
		i++;
	}
	return (before);
}

int	is_in_single_quotes(t_token *t)
{
	int	c;

	c = 0;
	if (t->content[c] == '\'')
	{
		while (t->content[c] != '\0')
			c++;
		if (t->content[c - 1] == '\'')
			return (1);
	}
	return (0);
}

void	expand_dollars(t_token **list, char **env)
{
	t_token	*t;
	int		c;
	int		skip_this_one;

	t = *list;
	if (t->content == NULL)
		t = t->next;
	while (t != NULL)
	{
		skip_this_one = 0;
		if (is_in_single_quotes(t) == 1)
			skip_this_one = 1;
		if (skip_this_one != 1)
		{
			c = 0;
			while (t->content[c] != '\0')
			{
				if (t->content[c] == '$')
					c = dollar_spotted(t, env, c);
				c++;
			}
		}
		t = t->next;
	}
	return ;
}


/*int	main(int arc, char *argv[], char *env[])
{
	t_token	*token;

	token = ft_calloc(1, sizeof(t_token));

	token->content = ft_calloc(22, sizeof(char));
	token->content[0] = 't';
	token->content[1] = 'h';
	token->content[2] = 'i';
	token->content[3] = 's';
	token->content[4] = 'i';
	token->content[5] = 's';
	token->content[6] = '$';
	token->content[7] = 'U';
	token->content[8] = 'S';
	token->content[9] = 'E';
	token->content[10] = 'R';
	token->content[11] = '$';
	token->content[12] = 'U';
	token->content[13] = 'S';
	token->content[14] = 'E';
	token->content[15] = 'R';
	token->content[16] = ' ';
	token->content[17] = 't';
	token->content[18] = 'e';
	token->content[19] = 's';
	token->content[20] = 't';
	token->content[21] = '\0';

	token->next = NULL;
	expand_dollars(&token, env);
	ft_printf("%s\n", token->content);
	free(token->content);
	free(token);
	return (0);
} */
