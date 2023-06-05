/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   proto_lexer_additional_functions.c                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nreher <nreher@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/03 10:38:35 by nreher            #+#    #+#             */
/*   Updated: 2023/05/25 12:49:18 by nreher           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_defs	make_defs(void)
{
	t_defs	defs;

	defs.blanks[0] = ' ';
	defs.blanks[1] = '	';
	defs.blanks[2] = '\0';
	defs.metachars[0] = "|";
	defs.metachars[1] = "<";
	defs.metachars[2] = ">";
	defs.metachars[3] = "<<";
	defs.metachars[4] = ">>";
	defs.metachars[5] = NULL;
	defs.seperators[0] = ">>";
	defs.seperators[1] = "<<";
	defs.seperators[2] = ">";
	defs.seperators[3] = "<";
	defs.seperators[4] = "|";
	defs.seperators[5] = "	";
	defs.seperators[6] = " ";
	defs.seperators[7] = NULL;
	return (defs);
}

void	token_add_back(t_token **token, t_token *new)
{
	t_token	*temp;

	if (*token == NULL)
	{
		*token = new;
		return ;
	}
	temp = *token;
	while (temp -> next != NULL)
	{
		temp = temp -> next;
	}
	temp -> next = new;
}

t_token	*new_token(char *content, int type)
{
	t_token	*out;

	out = ft_calloc(1, sizeof(t_token));
	if (out == NULL)
		return (NULL);
	out->content = ft_strdup(content);
	out->type = type;
	return (out);
}

void	fuck_norminete(t_sain *sain, t_defs defs, t_token **list, char *string)
{
	sain->i = is_current_delim(defs, sain->substring);
	if (sain->i != 0)
		pushcurrentsub(sain, string, list, defs);
	else
		sain->substring[sain->k++] = string[sain->c++];
}

