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

// ast creation:
void	create_cmd_node(t_node **cmd_node)
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

void	nullify_cmd_node(t_node **cmd_node)
{
	printf("NULLifying command node - without this the next command node won't be created\n");
	*cmd_node = NULL;
}

void	create_pipe_node(t_node **pipe_node, t_node **cmd_or_pipe_node)
{
	printf("creating pipe node and linking previous command node to left branch\n");
	(*pipe_node) = malloc(sizeof(t_node));
	(*pipe_node)->type = N_PIPE;
	(*pipe_node)->left = *cmd_or_pipe_node;
	(*pipe_node)->right = NULL;
	(*pipe_node)->cmd_elements = NULL;
	(*pipe_node)->infile = NULL;
	(*pipe_node)->outfile = NULL;
	if ((*cmd_or_pipe_node)->type == N_CMD)
		nullify_cmd_node(cmd_or_pipe_node);
}

void	link_pipe_node_right_branch(t_node **pipe_node, t_node **cmd_node)
{
	printf("linking command node to right branch of pipe_node\n");
	(*pipe_node)->right = *cmd_node;
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

void	add_command_element(t_node **cmd_node, char *content)
{
	printf("adding \"%s\" to cmd_elements list of command node\n", content);
	ft_lstadd_back(&(*cmd_node)->cmd_elements, ft_lstnew(content));
}

// parsing:
bool	parse__redirection(t_token **token, t_node **cmd_node)
{
	if ((*token) && (*token)->type == T_RETO)
	{
		(*token) = (*token)->next;
		if ((*token) && (*token)->type == T_WORD)
		{
			assign_redirection_to(cmd_node, (*token)->content);
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
			assign_redirection_from(cmd_node, (*token)->content);
			(*token) = (*token)->next;
			return (true);
		}
		return (false);
	}
	return (false);
}

bool	parse__simple_command_element(t_token **token, t_node **cmd_node)
{
	if ((*token) && (*token)->type == T_WORD)
	{
		(*token) = (*token)->next;
		return (true);
	}
	return (parse__redirection(token, cmd_node));
}

// prototype:
bool	parse__simple_command_tail(t_token **token, t_node **pipe_node, t_node **cmd_node);

// <SIMPLE-COMMAND>      ::= <SIMPLE-COMMAND-ELEMENT> <SIMPLE-COMMAND-TAIL>
bool	parse__simple_command(t_token **token, t_node **pipe_node, t_node **cmd_node)
{
	if (parse__simple_command_element(token, cmd_node))
	{
		return (parse__simple_command_tail(token, pipe_node, cmd_node));
	}
	return (false);
}

// <SIMPLE-COMMAND-TAIL> ::= <SIMPLE-COMMAND-ELEMENT> <SIMPLE-COMMAND-TAIL> | ε
bool	parse__simple_command_tail(t_token **token, t_node **pipe_node, t_node **cmd_node)
{
	if (parse__simple_command_element(token, cmd_node))
	{
		return (parse__simple_command_tail(token, pipe_node, cmd_node));
	}
	return (true); // ε (empty production)
}

// prototype:
bool	parse__pipeline_tail(t_token **token, t_node **pipe_node, t_node **cmd_node);

// <PIPELINE> 				::= <SIMPLE-COMMAND> <PIPELINE-TAIL>
bool	parse__pipeline(t_token **token, t_node **pipe_node, t_node **cmd_node)
{
	if (parse__simple_command(token, pipe_node, cmd_node))
	{
		return (parse__pipeline_tail(token, pipe_node, cmd_node));
	}
	return (false);
}

// <PIPELINE-TAIL>			::= 'T_PIPE' <PIPELINE> | ε
bool	parse__pipeline_tail(t_token **token, t_node **pipe_node, t_node **cmd_node)
{
	if ((*token) && (*token)->type == T_PIPE)
	{
		(*token) = (*token)->next;
		return (parse__pipeline(token, pipe_node, cmd_node));
	}
	return (true); // ε (empty production)
}
