// TODO: print ast properly (cmd are currently not printed, maybe because they have been null(ified))
// TODO: handle case when left hand branch of pipe is not a cmd but instead another pipe

// <REDIRECTION>			::=  'T_RETO' 'T_WORD'
//							|  'T_REFROM' 'T_WORD'

// <SIMPLE-COMMAND-ELEMENT>	::= 'T_WORD'
//							|  <REDIRECTION>

// <SIMPLE-COMMAND>			::=  <SIMPLE-COMMAND-ELEMENT>
// 							|  <SIMPLE-COMMAND> <SIMPLE-COMMAND-ELEMENT>

// <PIPELINE> 				::=
//							<PIPELINE> 'T_PIPE' <PIPELINE>
//							|  <SIMPLE-COMMAND>

// cc -Wall -Werror -Wextra -Ilibft -lreadline microparser-standalone-new-grammar.c libft/ft_strncmp.c libft/ft_strjoin.c libft/ft_split.c libft/ft_substr.c libft/ft_strlen.c

#include "minishell.h"

static	t_token	*ft_tokenlstfindlast(t_token *lst)
{
	if (!lst->next)
		return (lst);
	return (ft_tokenlstfindlast(lst->next));
}

t_token	*ft_tokenlstlast(t_token *lst)
{
	if (lst)
		return (ft_tokenlstfindlast(lst));
	return (NULL);
}

// used to print the full token list e.g. "ls -l | cat -e"
void	print_tokens(t_token *token)
{
	while (token)
	{
		printf("\"%s\" %s\n", token->content, tokentype_lookup(token->type));
		token = token->next;
	}
	printf("\n");
}

char	*tokentype_lookup(int type_num)
{
	char	*type_str;

	type_str = NULL;
	if (type_num == 0)
		type_str = "T_WORD";
	else if (type_num == 1)
		type_str = "T_PIPE";
	else if (type_num == 2)
		type_str = "T_REFROM";
	else if (type_num == 3)
		type_str = "T_RETO";
	else if (type_num == 4)
		type_str = "T_REFROM_HEREDOC";
	else if (type_num == 5)
		type_str = "T_RETO_APPEND";
	return (type_str);
}

t_node	*create_cmd_node(void)
{
	t_node	*cmd_node;

	cmd_node = malloc(sizeof(t_node));
	cmd_node->type = N_CMD;
	cmd_node->left = NULL;
	cmd_node->right = NULL;
	cmd_node->command_elements = NULL;
	cmd_node->infile = NULL;
	cmd_node->outfile = NULL;
	return (cmd_node);
}

void	nullify_cmd_node(t_node **cmd_node)
{
	printf("NULLifying command node - without this the next command node won't be created\n");
	*cmd_node = NULL;
}

t_node	*create_pipe_node(void)
{
	t_node	*pipe_node;

	pipe_node = malloc(sizeof(t_node));
	pipe_node->type = N_PIPE;
	pipe_node->left = NULL;
	pipe_node->right = NULL;
	pipe_node->command_elements = NULL;
	pipe_node->infile = NULL;
	pipe_node->outfile = NULL;
	return (pipe_node);
}

void	assign_redirection_from(t_node **cmd_node, char *filename)
{
	printf("setting redirection from: \"%s\"\n", filename);
	(*cmd_node)->infile = filename;
}

void	assign_redirection_to(t_node **cmd_node, char *filename)
{
	printf("setting redirection to: \"%s\"\n", filename);	
	(*cmd_node)->outfile = filename;
}

// parsing:
bool	parse__redirection(t_token **token, t_node ***ast_head)
{
	if ((*token) && (*token)->type == T_RETO)
	{
		(*token) = (*token)->next;
		if ((*token) && (*token)->type == T_WORD)
		{
			assign_redirection_to(*ast_head, (*token)->content);
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
			assign_redirection_from(*ast_head, (*token)->content);
			(*token) = (*token)->next;
			return (true);
		}
		return (false);
	}
	return (false);
}

bool	parse__simple_command_element(t_token **token, t_node ***ast_head, t_list **command_elements)
{
	if ((*token) && (*token)->type == T_WORD)
	{
		ft_lstadd_back(command_elements, ft_lstnew((*token)->content));
		(*token) = (*token)->next;
		return (true);
	}
	return (parse__redirection(token, ast_head));
}

// prototype:
bool	parse__simple_command_tail(t_token **token, t_node ***ast_head, t_list **command_elements);

// <SIMPLE-COMMAND>      ::= <SIMPLE-COMMAND-ELEMENT> <SIMPLE-COMMAND-TAIL>
bool	parse__simple_command(t_token **token, t_node ***ast_head)
{
	t_list	*command_elements;
	
	command_elements = NULL;
	if (parse__simple_command_element(token, ast_head, &command_elements))
	{
		return (parse__simple_command_tail(token, ast_head, &command_elements));
	}
	return (false);
}

void	link_next_command_node_into_tree(t_node ***ast_head, t_node *node_to_link)
{
	if ((*ast_head) == NULL)
	{
		(*ast_head) = &node_to_link;
		return ;
	}
	else
	{
		(**ast_head)->right = node_to_link;
		return ;
	}
	// while((*ast_head)->left != NULL)
	// {
	// 	if ((*ast_head)->type == N_PIPE && (*ast_head)->right == NULL)
	// 	{
	// 		(*ast_head)->right = node_to_link;
	// 		return ;
	// 	}
	// 	(*ast_head)->left = (*ast_head);
	// }
}

// <SIMPLE-COMMAND-TAIL> ::= <SIMPLE-COMMAND-ELEMENT> <SIMPLE-COMMAND-TAIL> | ε
bool	parse__simple_command_tail(t_token **token, t_node ***ast_head, t_list **command_elements)
{
	t_node	*node_to_link;

	if (parse__simple_command_element(token, ast_head, command_elements))
	{
		return (parse__simple_command_tail(token, ast_head, command_elements));
	}
	node_to_link = create_cmd_node(); // create: t_node *command_node (type = CMD) 
	node_to_link->command_elements = *command_elements; // link command_elements to command_node
	*command_elements = NULL; // make next command_elements.
	link_next_command_node_into_tree(ast_head, node_to_link); // if (pipe_node with free right side exists) link command_node to right side of pipe_node.
	return (true); // ε (empty production)
} 

// prototype:
bool	parse__pipeline_tail(t_token **token, t_node ***ast_head);

// <PIPELINE> 				::= <SIMPLE-COMMAND> <PIPELINE-TAIL>
bool	parse__pipeline(t_token **token, t_node ***ast_head)
{
	if (parse__simple_command(token, ast_head))
	{
		return (parse__pipeline_tail(token, ast_head));
	}
	return (false);
}

// <PIPELINE-TAIL>			::= 'T_PIPE' <PIPELINE> | ε
bool	parse__pipeline_tail(t_token **token, t_node ***ast_head)
{
	t_node	*pipe_node;

	if ((*token) && (*token)->type == T_PIPE)
	{
		(*token) = (*token)->next;
		pipe_node = create_pipe_node(); // create (new) pipe_node,  CREATING THE PIPE_NODE SOMEHOW OVERRIDES THE **ast_head, WHYYYYYYYY?
		pipe_node->left = **ast_head;
		*ast_head = &pipe_node;
		return (parse__pipeline(token, ast_head));
	}
	return (true); // ε (empty production)
}
