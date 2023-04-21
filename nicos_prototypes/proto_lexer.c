/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   proto_lexer.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nreher <nreher@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/20 08:34:58 by nreher            #+#    #+#             */
/*   Updated: 2023/04/21 09:25:43 by nreher           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "proto.h"

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
	defs.seperators[0] = " ";
	defs.seperators[1] = "	";
	defs.seperators[2] = "|";
	defs.seperators[3] = "<";
	defs.seperators[4] = ">";
	defs.seperators[5] = "<<";
	defs.seperators[6] = ">>";
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
	out->content = content;
	out->type = type;
	return (out);
}

t_token	*linker(char **tokens, char **metachars)
{
	int		c;
	int		type;
	t_token	*token_list;

	c = 0;
	token_list = ft_calloc(1, sizeof(t_token));
	while (tokens[c] != NULL)
	{
		type = T_WORD;
		if (ft_strncmp(tokens[c], metachars[0]
				, ft_strlen(tokens[c])) == 0)
			type = T_PIPE;
		else if (ft_strncmp(tokens[c], metachars[1]
				, ft_strlen(tokens[c])) == 0)
			type = T_RE_FROM;
		else if (ft_strncmp(tokens[c], metachars[2]
				, ft_strlen(tokens[c])) == 0)
			type = T_RE_TO;
		else if (ft_strncmp(tokens[c], metachars[3]
				, ft_strlen(tokens[c])) == 0)
			type = T_RE_FROM_HEREDOC;
		else if (ft_strncmp(tokens[c], metachars[4]
				, ft_strlen(tokens[c])) == 0)
			type = T_RE_TO_APPEND;
		token_add_back(&token_list, new_token(tokens[c], type));
		c++;
	}
	return (token_list);
}

t_token	*lexer(char *in)
{
	char	**tokens;
	t_defs	defs;
	t_token	*token_list;

	defs = make_defs();
	tokens = ft_split2(in, defs.seperators);
	token_list = linker(tokens, defs.metachars);
	return (token_list);
}

int	main(void)
{
	t_token	*token_list;

	token_list = lexer(">out  < in cat <in2");
	while (token_list->next != NULL)
	{
		ft_printf("content: %s	type: %d\n",token_list->content, token_list->type);
		token_list = token_list->next;
	}
}
