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


char	*make_searched(t_token *t, int *c)
{
	char	*searched;
	int		i;
	int		j;

	i = *c + 1;
	while (ft_isalnum(t->content[i]) == 1)
		i++;
	if (t->content[*c + 1] == '?')
		i = *c + 2;
	searched = ft_calloc(i - *c + 1, sizeof(char));
	(*c)++;
	j = 0;
	while (*c < i)
		searched[j++] = t->content[(*c)++];
	if (searched[0] != '\0')
		searched[j] = '=';
	return (searched);
}

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

int	is_surrounded_by(t_token *t, char a)
{
	int	c;

	c = 0;
	if (t->content[c] == a && ft_strlen(t->content) != 1)
	{
		while (t->content[c] != '\0')
			c++;
		if (t->content[c - 1] == a)
		{
			strip_quotes(t, a);
			return (1);
		}
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
		skip_this_one = is_surrounded_by(t, '\'');
		if (skip_this_one != 1)
		{
			is_surrounded_by(t, '"');
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
