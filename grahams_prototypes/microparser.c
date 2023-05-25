// <REDIRECTION>			::=  'T_RETO' 'T_WORD'
//							|  'T_REFROM' 'T_WORD'

// <SIMPLE-COMMAND-ELEMENT>	::= 'T_WORD'
//							|  <REDIRECTION>

// <SIMPLE-COMMAND>			::=  <SIMPLE-COMMAND-ELEMENT>
// 							|  <SIMPLE-COMMAND> <SIMPLE-COMMAND-ELEMENT>

// <PIPELINE> 				::=
//							<PIPELINE> 'T_PIPE' <PIPELINE>
//							|  <SIMPLE-COMMAND>

// EXAMPLE 1:
// < infile grep d | grep p > outfile

// <			infile			grep
// T_REFROM	T_WORD			
// [REDIRECTION 		]
// [SIMPLE-COMMAND-ELEMENT]
// [SIMPLE-COMMAND]
// [PIPELINE]

// cc -Wall -Werror -Wextra -Ilibft -lreadline microparser-standalone-new-grammar.c libft/ft_strncmp.c libft/ft_strjoin.c libft/ft_split.c libft/ft_substr.c libft/ft_strlen.c

#include "micro.h"

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

t_node *create_cmd_node(t_token *cmd_elements)
{
	t_node *cmd_node;

	cmd_node = malloc(sizeof(t_node));
	cmd_node->type = N_CMD;
	cmd_node->left = NULL;
	cmd_node->right = NULL;
	cmd_node->cmd_elements = cmd_elements;
	cmd_node->infile = NULL;
	cmd_node->outfile = NULL;
	return (cmd_node);
}

t_node *create_pipe_node(t_node *left_node, t_node *right_node)
{
	t_node *pipe_node;

	pipe_node = malloc(sizeof(t_node));
	pipe_node->type = N_PIPE;
	pipe_node->left = left_node;
	pipe_node->right = right_node;
	pipe_node->cmd_elements = NULL;
	pipe_node->infile = NULL;
	pipe_node->outfile = NULL;
	return (pipe_node);
}

// can be used to print either the full token list e.g. "ls -l | cat -e"
// or individual commands e.g. "ls -l"
void	print_tokens(t_token *token)
{
	while(token)
	{
		printf("\"%s\" %s\n", token->content, tokentype_lookup(token->type));
		token = token->next;
	}
	printf("\n");
}

// to print the ast 
void print_ast(t_node *ast)
{
	if (ast == NULL)
		return ;
	print_ast(ast->left);
	if (ast->type == N_PIPE)
		printf("|\n");
	print_ast(ast->right);
	if (ast->type == N_CMD)
		print_tokens(ast->cmd_elements);
}

void set_pipeline(t_token *token)
{
	printf("pipeline \"%s\"\n", token->content);
}

void set_command(t_token *token)
{
	printf("\"%s\"\n", token->content);
}

void set_redirect_to(t_token *token)
{
	printf("redirection to \"%s\"\n", token->content);
}

void set_redirect_from(t_token *token)
{
	printf("redirect from \"%s\"\n", token->content);
}

bool parse__redirection(t_token **token)
{
	if ((*token) && (*token)->type == T_RETO)
	{
		(*token) = (*token)->next;
		if ((*token) && (*token)->type == T_WORD)
		{
			set_redirect_to(*token);
			return true;
		}
		return false;
	}
	else if ((*token) && (*token)->type == T_REFROM)
	{
		(*token) = (*token)->next;
		if ((*token) && (*token)->type == T_WORD)
		{
			set_redirect_from(*token);
			return true;
		}
		return false;
	}
	return false;
}

bool parse__simple_command_element(t_token **token)
{
	if ((*token) && (*token)->type == T_WORD)
	{
		(*token) = (*token)->next;
		return true;
	}
	return(parse__redirection(token));
}

// prototype:
bool parse__simple_command_tail(t_token **token);

// <SIMPLE-COMMAND>      ::= <SIMPLE-COMMAND-ELEMENT> <SIMPLE-COMMAND-TAIL>
bool parse__simple_command(t_token **token)
{
	if (parse__simple_command_element(token))
	{
		return(parse__simple_command_tail(token));
	}
	return false;
}

// <SIMPLE-COMMAND-TAIL> ::= <SIMPLE-COMMAND-ELEMENT> <SIMPLE-COMMAND-TAIL> | ε
bool parse__simple_command_tail(t_token **token)
{
	if (parse__simple_command_element(token))
	{
		return(parse__simple_command_tail(token));
	}
	// if (*token)
	// 	set_command(*token);
	return true; // ε (empty production)
}

// prototype:
bool parse__pipeline_tail(t_token **token);

// <PIPELINE> 				::= <SIMPLE-COMMAND> <PIPELINE-TAIL>
bool parse__pipeline(t_token **token)
{
	if (parse__simple_command(token))
	{
		return(parse__pipeline_tail(token));
	}
	return false;
}

// <PIPELINE-TAIL>			::= 'T_PIPE' <PIPELINE> | ε
bool parse__pipeline_tail(t_token **token)
{
	if ((*token) && (*token)->type == T_PIPE)
	{
		(*token) = (*token)->next;
		return(parse__pipeline(token));
	}
	// if (*token)
	// 	set_pipeline(*token);
	return true; // ε (empty production)
}
