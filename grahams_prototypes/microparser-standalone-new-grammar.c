#include "micro.h"

t_token_types	catego_toketype(char *content)
{
	if (!ft_strncmp(content, "|", 1))
		return (T_PIPE);
	else if (!ft_strncmp(content, "<", 1))
		return (T_REFROM);
	else if (!ft_strncmp(content, ">", 1))
		return (T_RETO);
	else	
		return (T_WORD);
}

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

void	ft_tokenadd_back(t_token **lst, t_token *new)
{
	t_token	*current;

	if (!(*lst))
	{
		(*lst) = new;
		return ;
	}
	current = (*lst);
	while (current->next)
	{
		current = current->next;
	}
	current->next = new;
}

t_token	*ft_newtoken(void *content)
{
	t_token		*re;

	re = (t_token *)malloc(sizeof(t_token));
	if (!re)
		return (NULL);
	re->content = content;
	re->type = catego_toketype(content);
	re->next = NULL;
	return (re);
}

t_node *create_cmd_node(t_token *cmd)
{
	t_node *cmd_node;

	cmd_node = malloc(sizeof(t_node));
	cmd_node->type = N_CMD;
	cmd_node->left = NULL;
	cmd_node->right = NULL;
	cmd_node->cmd = cmd;
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
	pipe_node->cmd = NULL;
	pipe_node->infile = NULL;
	pipe_node->outfile = NULL;
	return (pipe_node);
}

char *tokentype_lookup(int type_num)
{
	char *type_str;

	type_str = NULL;
	if (type_num == 0)
		type_str = "T_WORD";
	else if (type_num == 1)
		type_str = "T_REFROM";
	else if (type_num == 2)
		type_str = "T_RETO";
	else if (type_num == 3)
		type_str = "T_PIPE";
	return(type_str);
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
		print_tokens(ast->cmd);
}

// should redirection be given a node?
void set_pipeline(t_token *token)
{
	printf("pipeline \"%s\"\n", token->content);
}

void set_command(t_token *token)
{
	printf("command \"%s\"\n", token->content);
}

void set_redirect_to(t_token *token)
{
	printf("redirect to \"%s\"\n", token->content);
}

void set_redirect_from(t_token *token)
{
	printf("redirect from \"%s\"\n", token->content);
}

t_node *create_cmd_node(t_token *cmd)
{
	t_node *cmd_node;

	cmd_node = malloc(sizeof(t_node));
	cmd_node->type = N_CMD;
	cmd_node->left = NULL;
	cmd_node->right = NULL;
	cmd_node->cmd = cmd;
	return (cmd_node);
}

t_node *create_pipe_node(t_node *left_node, t_node *right_node)
{
	t_node *pipe_node;

	pipe_node = malloc(sizeof(t_node));
	pipe_node->type = N_PIPE;
	pipe_node->left = left_node;
	pipe_node->right = right_node;
	pipe_node->cmd = NULL;
	return (pipe_node);
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
	if (*token)
		set_command(*token);
	return true; // ε (empty production)
}

// <PIPELINE> 				::=
//							<PIPELINE> 'T_PIPE' <PIPELINE>
//							|  <SIMPLE-COMMAND>

// prototype:
bool parse__pipeline_tail(t_token **token);

// <PIPELINE> 				::= <SIMPLE-COMMAND> <PIPELINE-TAIL>
bool parse__pipeline(t_token **token)
{
	printf("%s\n", (*token)->content);
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
	if (*token)
		set_pipeline(*token);
	return true; // ε (empty production)
}

void	fake_lexer_redirection_to(t_token **tokens)
{
	ft_tokenadd_back(tokens, ft_newtoken(">"));
	ft_tokenlstlast(*tokens)->type = T_RETO;
	ft_tokenadd_back(tokens, ft_newtoken("outfile"));
	ft_tokenlstlast(*tokens)->type = T_WORD;
}

void	fake_lexer__simple_command_element(t_token **tokens)
{
	ft_tokenadd_back(tokens, ft_newtoken("ls"));
	ft_tokenlstlast(*tokens)->type = T_WORD;
	ft_tokenadd_back(tokens, ft_newtoken("<"));
	ft_tokenlstlast(*tokens)->type = T_REFROM;
	ft_tokenadd_back(tokens, ft_newtoken("outfile"));
	ft_tokenlstlast(*tokens)->type = T_WORD;
	ft_tokenadd_back(tokens, ft_newtoken("another-cmd"));
	ft_tokenlstlast(*tokens)->type = T_WORD;	
}

void	fake_lexer__simple_command(t_token **tokens)
{
	ft_tokenadd_back(tokens, ft_newtoken("<"));
	ft_tokenlstlast(*tokens)->type = T_REFROM;
	ft_tokenadd_back(tokens, ft_newtoken("infile"));
	ft_tokenlstlast(*tokens)->type = T_WORD;
	ft_tokenadd_back(tokens, ft_newtoken("wc"));
	ft_tokenlstlast(*tokens)->type = T_WORD;
	ft_tokenadd_back(tokens, ft_newtoken(">"));
	ft_tokenlstlast(*tokens)->type = T_REFROM;
	ft_tokenadd_back(tokens, ft_newtoken("outfile"));
	ft_tokenlstlast(*tokens)->type = T_WORD;
}

void	fake_lexer__pipeline(t_token **tokens)
{
	ft_tokenadd_back(tokens, ft_newtoken("ls"));
	ft_tokenlstlast(*tokens)->type = T_WORD;
	ft_tokenadd_back(tokens, ft_newtoken("|"));
	ft_tokenlstlast(*tokens)->type = T_PIPE;
	ft_tokenadd_back(tokens, ft_newtoken("wc"));
	ft_tokenlstlast(*tokens)->type = T_WORD;
}

int main()
{
	t_token	*tokens;
	// t_node *ast;

	tokens = NULL;
	// EXAMPLE 1: simple command element
	// fake_lexer__simple_command_element(&tokens);
	// printf("%i ", parse__simple_command_element(&tokens));
	// printf("%s ", tokens->content);
	// printf("%i ", parse__simple_command_element(&tokens));
	// printf("%s\n", tokens->content);	
	// printf("%i\n", parse__simple_command_element(&tokens));
	// if (tokens)
	// 	printf("%s ", tokens->content);
	// else
	// 	printf("(NULL)\n");

	// EXAMPLE 2: simple command
	// fake_lexer__simple_command(&tokens);
	// printf("%i\n", parse__simple_command(&tokens));
	// if (tokens)
	// 	printf("%s ", tokens->content);
	// else
	// 	printf("(NULL)\n");

	// EXAMPLE 3: pipeline
	fake_lexer__pipeline(&tokens);
	printf("%i\n", parse__pipeline(&tokens));
	if (tokens)
		printf("%s ", tokens->content);
	else
		printf("(NULL)\n");

	// ast = 
	// if (!parse__pipeline(tokens))
	// 	printf("syntax-error\n");
	// if (ast)
	// {
	// 	print_ast(ast);
	// }
	return false;
}