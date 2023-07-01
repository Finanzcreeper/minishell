#include "minishell.h"

int	parse__simple_command_element(t_token **token,
	t_list **command_elements, t_redirs *redirs)
{
	if ((*token) && (*token)->type == T_WORD)
	{
		ft_lstadd_back(command_elements, ft_lstnew((*token)->content));
		(*token) = (*token)->next;
		return (0);
	}
	return (parse__redirection(token, redirs));
}

t_node	*make_cmd_node(t_list **command_elements, t_redirs *redirs)
{
	t_node	*cmd_node;

	cmd_node = ft_calloc(1, sizeof(t_node));
	cmd_node->command_elements = *command_elements;
	cmd_node->infile = redirs->infile;
	cmd_node->outfile = redirs->outfile;
	cmd_node->append_when_writing = redirs->append_when_writing;
	cmd_node->read_from_heredoc = redirs->read_from_heredoc;
	cmd_node->limiter = redirs->limiter;
	*command_elements = NULL;
	ft_bzero(redirs, sizeof(t_redirs));
	return (cmd_node);
}

bool	parse__simple_command_tail(t_token **token,
	t_node *ast_head, t_list **command_elements, t_redirs *redirs)
{
	t_node	*cmd_node;
	int		err;

	err = parse__simple_command_element(token, command_elements, redirs);
	if (err == 0)
	{
		return (parse__simple_command_tail(token, ast_head,
				command_elements, redirs));
	}
	if (err == 2)
	{
		ft_lstclear(command_elements, free);
		command_elements = NULL;
		return (false);
	}
	cmd_node = make_cmd_node(command_elements, redirs);
	if (ast_head->type != 1)
	{
		*ast_head = *cmd_node;
		free(cmd_node);
	}
	else
		ast_head->right = cmd_node;
	return (true);
}

bool	parse__pipeline(t_token **token, t_node **ast_head)
{
	bool		spcmd;
	t_list		*command_elements;
	t_redirs	*redirs;

	redirs = ft_calloc(1, sizeof(t_redirs));
	command_elements = NULL;
	if (parse__simple_command_element(token, &command_elements, redirs) == 0)
	{
		spcmd = parse__simple_command_tail(token, *ast_head,
				&command_elements, redirs);
	}
	else
		spcmd = false;
	if (spcmd == true)
	{
		free(redirs);
		return (parse__pipeline_tail(token, ast_head));
	}
	free(redirs);
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
