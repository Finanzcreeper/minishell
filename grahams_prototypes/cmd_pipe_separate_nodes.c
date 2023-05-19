// TODO next: re-integrate into 

// Create a parsing function for each non-terminal symbol in the grammar i.e. <pipeline> and <cmd>

// // <pipeline>	::= <cmd> {T_PIPE <cmd>}
// // <cmd>		::= T_WORD {T_WORD}

// Create a main parsing function corresponding to the start symbol of the grammar.
// The start symbol represents the initial non-terminal symbol from which the parsing process begins.
// (in my case <pipeline>)

// nodes can be of type PIPE or CMD
// but the problem is the left or right node can point to either a PIPE or CMD and we can't hold both
// so I used one node type, containing variables than can represent either a PIPE or CMD
// 		PIPE
// 		/	\
// 	PIPE	CMD
//    /
// CMD

#include <stdlib.h>
#include <stdio.h>

typedef enum e_token_types
{
	T_WORD,
	T_PIPE	
}					t_token_types;

typedef struct s_token
{
	int				type;
	char			*content;
	struct s_token	*next;
}					t_token;

typedef enum e_node_types
{
	N_CMD,
	N_PIPE,
}	e_node_types;

typedef struct s_node
{
	int				type;
	struct s_node	*left;
	struct s_node	*right;
	t_token			*cmd;
}					t_node;

static	t_token	*ft_lstfindlast(t_token *lst)
{
	if (!lst->next)
		return (lst);
	return (ft_lstfindlast(lst->next));
}

t_token	*ft_lstlast(t_token *lst)
{
	if (lst)
		return (ft_lstfindlast(lst));
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
	while (current->next != 0)
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
	re->next = 0;
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

// can be used to print either the full token list e.g. "ls -l | cat -e"
// or individual commands e.g. "ls -l"
void	print_tokens(t_token *token)
{
	while(token)
	{
		printf("%s ", token->content);
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

void	fake_lexer_one_command(t_token **tokens)
{
	ft_tokenadd_back(tokens, ft_newtoken("ls"));
	ft_lstlast(*tokens)->type = T_WORD;
	ft_tokenadd_back(tokens, ft_newtoken("-l"));
	ft_lstlast(*tokens)->type = T_WORD;
}

void	fake_lexer_two_commands(t_token **tokens)
{
	ft_tokenadd_back(tokens, ft_newtoken("ls"));
	ft_lstlast(*tokens)->type = T_WORD;
	ft_tokenadd_back(tokens, ft_newtoken("-l"));
	ft_lstlast(*tokens)->type = T_WORD;
	ft_tokenadd_back(tokens, ft_newtoken("|"));
	ft_lstlast(*tokens)->type = T_PIPE;
	ft_tokenadd_back(tokens, ft_newtoken("cat"));
	ft_lstlast(*tokens)->type = T_WORD;
	ft_tokenadd_back(tokens, ft_newtoken("-e"));
	ft_lstlast(*tokens)->type = T_WORD;
}

void	fake_lexer_three_commands(t_token **tokens)
{
	ft_tokenadd_back(tokens, ft_newtoken("ls"));
	ft_lstlast(*tokens)->type = T_WORD;
	ft_tokenadd_back(tokens, ft_newtoken("-l"));
	ft_lstlast(*tokens)->type = T_WORD;
	ft_tokenadd_back(tokens, ft_newtoken("|"));
	ft_lstlast(*tokens)->type = T_PIPE;
	ft_tokenadd_back(tokens, ft_newtoken("cat"));
	ft_lstlast(*tokens)->type = T_WORD;
	ft_tokenadd_back(tokens, ft_newtoken("-e"));
	ft_lstlast(*tokens)->type = T_WORD;
	ft_tokenadd_back(tokens, ft_newtoken("|"));
	ft_lstlast(*tokens)->type = T_PIPE;
	ft_tokenadd_back(tokens, ft_newtoken("bat"));
	ft_lstlast(*tokens)->type = T_WORD;
	ft_tokenadd_back(tokens, ft_newtoken("-b"));
	ft_lstlast(*tokens)->type = T_WORD;	
}

void	fake_lexer_start_pipe(t_token **tokens)
{
	ft_tokenadd_back(tokens, ft_newtoken("|"));
	ft_lstlast(*tokens)->type = T_PIPE;
	ft_tokenadd_back(tokens, ft_newtoken("-l"));
	ft_lstlast(*tokens)->type = T_WORD;
}

void	fake_lexer_double_pipe(t_token **tokens)
{
	ft_tokenadd_back(tokens, ft_newtoken("ls"));
	ft_lstlast(*tokens)->type = T_WORD;
	ft_tokenadd_back(tokens, ft_newtoken("-l"));
	ft_lstlast(*tokens)->type = T_WORD;
	ft_tokenadd_back(tokens, ft_newtoken("|"));
	ft_lstlast(*tokens)->type = T_PIPE;
	ft_tokenadd_back(tokens, ft_newtoken("|"));
	ft_lstlast(*tokens)->type = T_PIPE;
	ft_tokenadd_back(tokens, ft_newtoken("-e"));
	ft_lstlast(*tokens)->type = T_WORD;
}

void	fake_lexer_triple_pipe(t_token **tokens)
{
	ft_tokenadd_back(tokens, ft_newtoken("ls"));
	ft_lstlast(*tokens)->type = T_WORD;
	ft_tokenadd_back(tokens, ft_newtoken("-l"));
	ft_lstlast(*tokens)->type = T_WORD;
	ft_tokenadd_back(tokens, ft_newtoken("|"));
	ft_lstlast(*tokens)->type = T_PIPE;
	ft_tokenadd_back(tokens, ft_newtoken("|"));
	ft_lstlast(*tokens)->type = T_PIPE;
	ft_tokenadd_back(tokens, ft_newtoken("|"));
	ft_lstlast(*tokens)->type = T_PIPE;	
	ft_tokenadd_back(tokens, ft_newtoken("-e"));
	ft_lstlast(*tokens)->type = T_WORD;
}

void	fake_lexer_pipe_no_subseq_cmd(t_token **tokens)
{
	ft_tokenadd_back(tokens, ft_newtoken("ls"));
	ft_lstlast(*tokens)->type = T_WORD;
	ft_tokenadd_back(tokens, ft_newtoken("-l"));
	ft_lstlast(*tokens)->type = T_WORD;
	ft_tokenadd_back(tokens, ft_newtoken("|"));
	ft_lstlast(*tokens)->type = T_PIPE;
}

// implements the following line of the grammar: <cmd>		::= T_WORD {T_WORD}
t_node *parse_cmd(t_token **token)
{
	t_token *cmd; // linked list of one or more tokens representing the command ie in "ls -l"
	t_node *cmd_node;

	cmd = NULL;
	if ((*token)->type != T_WORD)
	{
		printf("syntax error-cmd\n");
		return (NULL);
	}
	while (*token && (*token)->type == T_WORD)
	{	
		ft_tokenadd_back(&cmd, ft_newtoken((*token)->content));
		*token = (*token)->next;
	}
	cmd_node = create_cmd_node(cmd);
	return (cmd_node);
}

// implements the following line of the grammar: <pipeline>	::= <cmd> {T_PIPE <cmd>}
// approach is to only create the pipe_node once the left and right branch exist
// (not as before when the right branch was initialised as NULL then right cmd created, then assigned to right branch of pipe)
// left hand branches can be cmds or pipes, right hand branches are always commands
t_node *parse_pipeline(t_token *token)
{
	t_node *cmd_node;
	t_node *pipe_node;

	cmd_node = parse_cmd(&token); // undefined behaviour
	if (!cmd_node)
		return (NULL);
	if (!token)
		return (cmd_node);
	pipe_node = cmd_node;
	while (token)
	{
		if (token->type != T_PIPE)
		{
			printf("syntax-error-pipe1\n");
			return (NULL);
		}
		token = token->next;
		if (!token)
		{
			printf("syntax-error-pipe2\n");
			return (NULL);
		}
		cmd_node = parse_cmd(&token);
		if (!cmd_node)
			return (NULL);
		// create pipe node
		pipe_node = create_pipe_node(pipe_node, cmd_node);
	}
	return (pipe_node);
}

int main()
{
	t_token	*tokens;
	t_node *ast;

	tokens = NULL;
	// fake_lexer(&tokens);
	// fake_lexer_one_command(&tokens);
	// fake_lexer_two_commands(&tokens);
	fake_lexer_three_commands(&tokens);
	// fake_lexer_start_pipe(&tokens);
	// fake_lexer_double_pipe(&tokens);
	// fake_lexer_triple_pipe(&tokens);	
	// fake_lexer_pipe_no_subseq_cmd(&tokens);

	ast = parse_pipeline(tokens);
	if (ast)
	{
		print_ast(ast);
	}
	return (0);
}