/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_dollar_handling.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nreher <nreher@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/06 14:44:31 by gbooth            #+#    #+#             */
/*   Updated: 2023/07/06 18:19:23 by nreher           ###   ########.fr       */
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
	while (ft_isalnum(t->content[i]) == 1 || t->content[i] == '_')
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
				if (t->content[c] == '$' && t->content[c + 1] != '\0')
					c = dollar_spotted(t, env, c);
				c++;
			}
		}
		t = t->next;
	}
	return ;
}
