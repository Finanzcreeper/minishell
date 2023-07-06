/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_2_redirection.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbooth <gbooth@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/06 14:45:15 by gbooth            #+#    #+#             */
/*   Updated: 2023/07/06 14:46:56 by gbooth           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	parse_reto(t_token **token, t_redirs *redirs)
{
	(*token) = (*token)->next;
	if ((*token) && (*token)->type == T_WORD)
	{
		redirs->outfile = (*token)->content;
		(*token) = (*token)->next;
		return (0);
	}
	return (2);
}

int	parse_refrom(t_token **token, t_redirs *redirs)
{
	(*token) = (*token)->next;
	if ((*token) && (*token)->type == T_WORD)
	{
		redirs->infile = (*token)->content;
		redirs->read_from_heredoc = false;
		(*token) = (*token)->next;
		return (0);
	}
	return (2);
}

int	parse_reto_append(t_token **token, t_redirs *redirs)
{
	(*token) = (*token)->next;
	if ((*token) && (*token)->type == T_WORD)
	{
		redirs->append_when_writing = true;
		redirs->outfile = (*token)->content;
		(*token) = (*token)->next;
		return (0);
	}
	return (2);
}

int	parse_refrom_heredoc(t_token **token, t_redirs *redirs)
{
	redirs->read_from_heredoc = true;
	(*token) = (*token)->next;
	if ((*token) && (*token)->type == T_WORD)
	{
		redirs->limiter = (*token)->content;
		(*token) = (*token)->next;
		return (0);
	}
	return (2);
}

int	parse__redirection(t_token **token, t_redirs *redirs)
{
	if ((*token) && (*token)->type == T_RETO)
	{
		return (parse_reto(token, redirs));
	}
	else if ((*token) && (*token)->type == T_REFROM)
	{
		return (parse_refrom(token, redirs));
	}
	else if ((*token) && (*token)->type == T_RETO_APPEND)
	{
		return (parse_reto_append(token, redirs));
	}
	else if ((*token) && (*token)->type == T_REFROM_HEREDOC)
	{
		return (parse_refrom_heredoc(token, redirs));
	}
	return (1);
}
