/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   grammar_proto.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nreher <nreher@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/21 12:19:08 by nreher            #+#    #+#             */
/*   Updated: 2023/05/02 16:57:21 by nreher           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "proto.h"

void	rule_add_back(t_rule **token, t_rule *new)
{
	t_rule	*temp;

	if (*token == NULL)
	{
		*token = new;
		return ;
	}
	temp = *token;
	while (temp->next_rule != NULL)
	{
		temp = temp->next_rule;
	}
	temp->next_rule = new;
}

t_rule	*new_rule(char *lhs, char *rhs)
{
	t_rule	*out;

	out = ft_calloc(1, sizeof(t_rule));
	if (out == NULL)
		return (NULL);
	out->lhs = lhs;
	out->rhs = ft_split(rhs, ' ');
	return (out);
}

void	create_rules(t_rule *rules)
{
	rule_add_back(rules, new_rule("E", "2E2"));
	rule_add_back(rules, new_rule("E", "3E3"));
	rule_add_back(rules, new_rule("E", "4"));
}

void	shift_reduce(t_rule *rules, int argc, char *input)
{
	char	**stack[argc];

	while (rules != NULL)
	{

	}
}

int	main(int argc, char *argv[])
{
	t_rule	*rules;

	create_rules(rules);
	shift_reduce(rules, argc, argv);
}

