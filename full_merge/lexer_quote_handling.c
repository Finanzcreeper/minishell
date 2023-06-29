
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

void	double_quoter(t_sain *sain, char *string, t_token **list, t_defs defs)
{
	while (string[sain->c] != '"' && string[sain->c] != '\0')
	{
		sain->substring[sain->k++] = string[sain->c++];
	}
	if (string[sain->c] != '\0')
		sain->substring[sain->k++] = string[sain->c++];
	pushcurrentsub(sain, string, list, defs);
}

void	single_quoter(t_sain *sain, char *string, t_token **list, t_defs defs)
{
	while (string[sain->c] != 39 && string[sain->c] != '\0')
	{
		sain->substring[sain->k++] = string[sain->c++];
	}
	if (string[sain->c] != '\0')
		sain->substring[sain->k++] = string[sain->c++];
	pushcurrentsub(sain, string, list, defs);
}

void	quote_handler(t_sain *sain, char *string, t_token **list, t_defs defs)
{
	int	c;

	c = sain->c + 1;
	if (string[sain->c] == '"')
	{
		while (string[c] != '"')
		{
			if (string[c++] == '\0')
				return ;
		}
		pushcurrentsub(sain, string, list, defs);
		sain->substring[sain->k++] = string[sain->c++];
		double_quoter(sain, string, list, defs);
	}
	if (string[sain->c] == '\'')
	{
		while (string[c] != '\'')
		{
			if (string[c++] == '\0')
				return ;
		}
		pushcurrentsub(sain, string, list, defs);
		sain->substring[sain->k++] = string[sain->c++];
		single_quoter(sain, string, list, defs);
	}
}
