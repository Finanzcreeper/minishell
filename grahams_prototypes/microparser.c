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

// used to print the full token list e.g. "ls -l | cat -e"
void	print_tokens(t_token *token)
{
	while(token)
	{
		printf("\"%s\" %s\n", token->content, tokentype_lookup(token->type));
		token = token->next;
	}
	printf("\n");
}

// ast creation:
void create_cmd_node(t_node **cmd_node)
{
	printf("creating empty command node\n");
	(*cmd_node) = malloc(sizeof(t_node));
	(*cmd_node)->type = N_CMD;
	(*cmd_node)->left = NULL;
	(*cmd_node)->right = NULL;
	(*cmd_node)->cmd_elements = NULL;
	(*cmd_node)->infile = NULL;
	(*cmd_node)->outfile = NULL;
}

void create_pipe_node(t_node **cmd_node)
{
	t_node pipe_node;

	printf("creating pipe node and linking previous command to left branch\n");
	pipe_node.type = N_PIPE;
	pipe_node.left = *cmd_node;
	pipe_node.right = NULL;
	pipe_node.cmd_elements = NULL;
	pipe_node.infile = NULL;
	pipe_node.outfile = NULL;
	*cmd_node = &pipe_node;
}

void assign_pipe_node_right_branch(t_node **pipe_node)
{
	(*pipe_node)->right = NULL;
}

void assign_redirection_from(t_node **cmd_node, char *filename)
{
	printf("setting redirection from: \"%s\"\n", filename);
	(*cmd_node)->infile = filename;
}

void assign_redirection_to(t_node **cmd_node, char *filename)
{
	printf("setting redirection to: \"%s\"\n", filename);	
	(*cmd_node)->outfile = filename;
}

void add_command_element(t_list *cmd, char *content)
{
	printf("adding \"%s\" to cmd_elements list of command node\n", content);
	ft_lstadd_back(&cmd, ft_lstnew(content));
}

// parsing:
bool parse__redirection(t_token **token, t_node **cmd_node)
{
	if ((*token) && (*token)->type == T_RETO)
	{
		(*token) = (*token)->next;
		if ((*token) && (*token)->type == T_WORD)
		{
			assign_redirection_to(cmd_node, (*token)->content);
			if (!(*cmd_node))
				create_cmd_node(cmd_node);
			(*token) = (*token)->next;
			return true;
		}
		return false;
	}
	else if ((*token) && (*token)->type == T_REFROM)
	{
		(*token) = (*token)->next;
		if ((*token) && (*token)->type == T_WORD)
		{
			assign_redirection_from(cmd_node, (*token)->content);
			if (!(*cmd_node))
				create_cmd_node(cmd_node);
			(*token) = (*token)->next;
			return true;
		}
		return false;
	}
	return false;
}

bool parse__simple_command_element(t_token **token, t_node **cmd_node)
{
	if ((*token) && (*token)->type == T_WORD)
	{
		if (!(*cmd_node))
			create_cmd_node(cmd_node);
		add_command_element((*cmd_node)->cmd_elements, (*token)->content);
		(*token) = (*token)->next;
		return true;
	}
	return(parse__redirection(token, cmd_node));
}

// prototype:
bool parse__simple_command_tail(t_token **token, t_node **ast_root);

// <SIMPLE-COMMAND>      ::= <SIMPLE-COMMAND-ELEMENT> <SIMPLE-COMMAND-TAIL>
bool parse__simple_command(t_token **token, t_node **ast_root)
{
	if (parse__simple_command_element(token, ast_root))
	{
		return(parse__simple_command_tail(token, ast_root));
	}
	return false;
}

// <SIMPLE-COMMAND-TAIL> ::= <SIMPLE-COMMAND-ELEMENT> <SIMPLE-COMMAND-TAIL> | ε
bool parse__simple_command_tail(t_token **token, t_node **ast_root)
{
	if (parse__simple_command_element(token, ast_root))
	{
		return(parse__simple_command_tail(token, ast_root));
	}
	printf("finished parsing simple command\n");
	if ((*ast_root)->type == N_PIPE)

	return true; // ε (empty production)
}

// prototype:
bool parse__pipeline_tail(t_token **token, t_node **ast_root);

// <PIPELINE> 				::= <SIMPLE-COMMAND> <PIPELINE-TAIL>
bool parse__pipeline(t_token **token, t_node **ast_root)
{
	if (parse__simple_command(token, ast_root))
	{
		return(parse__pipeline_tail(token, ast_root));
	}
	return false;
}

// <PIPELINE-TAIL>			::= 'T_PIPE' <PIPELINE> | ε
bool parse__pipeline_tail(t_token **token, t_node **ast_root)
{
	if ((*token) && (*token)->type == T_PIPE)
	{
		(*token) = (*token)->next;
		if (ast_root) // only create the pipe node if a previous command exists i.e. not if there's just one command
			create_pipe_node(ast_root);
		return(parse__pipeline(token, ast_root));
	}
	printf("finished parsing pipeline\n");
	return true; // ε (empty production)
}
