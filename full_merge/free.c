/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbooth <gbooth@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/06 14:43:06 by gbooth            #+#    #+#             */
/*   Updated: 2023/07/06 14:43:08 by gbooth           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_tokens(t_token *tokens_head)
{
	if (tokens_head == NULL)
		return ;
	free_tokens(tokens_head->next);
	free(tokens_head);
}

void	lex_freedman(t_token *tokens)
{
	t_token	*temp;

	while (tokens != NULL)
	{
		temp = tokens;
		tokens = tokens->next;
		if (temp->type != T_WORD)
			free(temp->content);
		free(temp);
	}
}

void	free_ast_node2(t_node *temp)
{
	if (temp->right != NULL)
	{
		free(temp->right);
		temp->right = NULL;
	}
	if (temp->cmdarr != NULL)
	{
		free(temp->cmdarr);
		temp->cmdarr = NULL;
	}
	if (temp->limiter != NULL)
	{
		free(temp->limiter);
		temp->limiter = NULL;
	}
	free(temp);
}

void	free_ast_node(t_node *temp)
{
	if (temp->command_elements != NULL)
	{
		ft_lstclear(&temp->command_elements, free);
		temp->command_elements = NULL;
	}
	if (temp->infile != NULL)
	{
		free(temp->infile);
		temp->infile = NULL;
	}
	if (temp->outfile != NULL)
	{
		free(temp->outfile);
		temp->outfile = NULL;
	}
	if (temp->left != NULL)
	{
		free(temp->left);
		temp->left = NULL;
	}
	free_ast_node2(temp);
}

void	free_ast(t_node *ast)
{
	if (ast == NULL)
		return ;
	if (ast->left != NULL)
	{
		free_ast(ast->left);
		ast->left = NULL;
	}
	if (ast->right != NULL)
	{
		free_ast(ast->right);
		ast->right = NULL;
	}
	free_ast_node(ast);
	return ;
}
