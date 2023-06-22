
#include "minishell.h"

bool	parse__redirection(t_token **token, t_node *ast_head)
{
	if ((*token) && (*token)->type == T_RETO)
	{
		(*token) = (*token)->next;
		if ((*token) && (*token)->type == T_WORD)
		{
			ast_head->outfile = (*token)->content;
			(*token) = (*token)->next;
			return (true);
		}
		return (false);
	}
	else if ((*token) && (*token)->type == T_REFROM)
	{
		(*token) = (*token)->next;
		if ((*token) && (*token)->type == T_WORD)
		{
			ast_head->infile = (*token)->content;
			(*token) = (*token)->next;
			return (true);
		}
		return (false);
	}
	else if ((*token) && (*token)->type == T_RETO_APPEND)
	{
		(*token) = (*token)->next;
		if ((*token) && (*token)->type == T_WORD)
		{
			ast_head->append_when_writing = true;
			ast_head->outfile = (*token)->content;
			(*token) = (*token)->next;
			return (true);
		}
		return (false);
	}
	else if ((*token) && (*token)->type == T_REFROM_HEREDOC)
	{
		ast_head->read_from_heredoc = true;
		(*token) = (*token)->next;
		if ((*token) && (*token)->type == T_WORD)
		{
			ast_head->limiter = (*token)->content;
			(*token) = (*token)->next;
			return (true);
		}
		return (false);
	}	
	return (false);
}

bool	parse__simple_command_element(t_token **token,
t_node *ast_head, t_list **command_elements)
{
	if ((*token) && (*token)->type == T_WORD)
	{
		ft_lstadd_back(command_elements, ft_lstnew((*token)->content));
		(*token) = (*token)->next;
		return (true);
	}
	return (parse__redirection(token, ast_head));
}

bool	parse__simple_command_tail(t_token **token,
	t_node *ast_head, t_list **command_elements)
{
	t_node	*cmd_node;

	if (parse__simple_command_element(token, ast_head, command_elements))
	{
		return (parse__simple_command_tail(token, ast_head, command_elements));
	}
	cmd_node = ft_calloc(1, sizeof(t_node));
	cmd_node->command_elements = *command_elements;
	*command_elements = NULL;
	if (ast_head->type != 1)
	{
		*ast_head = *cmd_node;
	}
	else
	{
		ast_head->right = cmd_node;
	}
	return (true);
}

bool	parse__pipeline(t_token **token, t_node **ast_head)
{
	bool	test;
	t_list	*command_elements;

	command_elements = NULL;
	if (parse__simple_command_element(token, *ast_head, &command_elements))
	{
		test = parse__simple_command_tail(token, *ast_head, &command_elements);
	}
	else
		test = false;
	if (test == true)
	{
		return (parse__pipeline_tail(token, ast_head));
	}
	return (false);
}

bool	parse__pipeline_tail(t_token **token, t_node **ast_head)
{
	t_node	*pipe_node;

	if ((*token) && (*token)->type == T_PIPE)
	{
		(*token) = (*token)->next;
		pipe_node = ft_calloc(1, sizeof(t_node));
		pipe_node->type = N_PIPE;
		pipe_node->left = *ast_head;
		*ast_head = pipe_node;
		return (parse__pipeline(token, ast_head));
	}
	return (true);
}
