// - Create a main parsing function corresponding to the start symbol of the grammar.
// - The main parsing function analyzes the input tokens and invokes the appropriate parsing functions based on the grammar rules.
// - Each parsing function handles the parsing logic for a specific non-terminal symbol by examining the current token and recursively invoking other parsing functions to match the grammar rules.
// - The parsing functions may also handle error conditions, such as encountering an unexpected token or failing to match a grammar rule.

// TODO: protect mallocs, free mallocs

// # example:
// ls -l | cat -e

// printf("syntax error\n");

#include <stdlib.h>
#include <stdio.h>

typedef enum e_token_types
{
	T_WORD = 0,
	T_PIPE	
}	t_token_types;

typedef struct s_token
{
	int				type;
	char			*content;
	struct s_token	*next;
}					t_token;

typedef enum e_ast_types
{
	A_CMD_WORD,
	A_PIPE_SEQUENCE,
}	t_ast_types;

typedef struct s_node
{
	int				type;
	char			*content;
	struct s_node	*left;
	struct s_node	*right;
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

t_token	*multipipe_test()
{
	t_token	*tokens;

	tokens = ft_newtoken("ls");
	tokens->type = T_WORD;
	ft_tokenadd_back(&tokens, ft_newtoken("-l"));
	ft_lstlast(tokens)->type = T_WORD;
	ft_tokenadd_back(&tokens, ft_newtoken("-m"));
	ft_lstlast(tokens)->type = T_WORD;
	ft_tokenadd_back(&tokens, ft_newtoken("-n"));
	ft_lstlast(tokens)->type = T_WORD;
	ft_tokenadd_back(&tokens, ft_newtoken("|"));
	ft_lstlast(tokens)->type = T_PIPE;
	ft_tokenadd_back(&tokens, ft_newtoken("cat"));
	ft_lstlast(tokens)->type = T_WORD;
	ft_tokenadd_back(&tokens, ft_newtoken("-e"));	
	ft_lstlast(tokens)->type = T_WORD;
	ft_tokenadd_back(&tokens, ft_newtoken("|"));
	ft_lstlast(tokens)->type = T_PIPE;
	ft_tokenadd_back(&tokens, ft_newtoken("twat"));
	ft_lstlast(tokens)->type = T_WORD;
	ft_tokenadd_back(&tokens, ft_newtoken("-b"));	
	ft_lstlast(tokens)->type = T_WORD;
	return(tokens);
}

t_token	*single_command_test()
{
	t_token	*tokens;

	tokens = ft_newtoken("ls");
	tokens->type = T_WORD;
	return(tokens);
}

t_token	*double_pipe_error_test()
{
	t_token	*tokens;

	tokens = ft_newtoken("ls");
	tokens->type = T_WORD;
	ft_tokenadd_back(&tokens, ft_newtoken("-l"));
	ft_lstlast(tokens)->type = T_WORD;
	ft_tokenadd_back(&tokens, ft_newtoken("-m"));
	ft_lstlast(tokens)->type = T_WORD;
	ft_tokenadd_back(&tokens, ft_newtoken("-n"));
	ft_lstlast(tokens)->type = T_WORD;
	ft_tokenadd_back(&tokens, ft_newtoken("|"));
	ft_lstlast(tokens)->type = T_PIPE;
	ft_tokenadd_back(&tokens, ft_newtoken("|"));
	ft_lstlast(tokens)->type = T_PIPE;
	ft_tokenadd_back(&tokens, ft_newtoken("-e"));	
	ft_lstlast(tokens)->type = T_WORD;
	ft_tokenadd_back(&tokens, ft_newtoken("|"));
	ft_lstlast(tokens)->type = T_PIPE;
	ft_tokenadd_back(&tokens, ft_newtoken("twat"));
	ft_lstlast(tokens)->type = T_WORD;
	ft_tokenadd_back(&tokens, ft_newtoken("-b"));	
	ft_lstlast(tokens)->type = T_WORD;
	return(tokens);
}

t_token	*pipe_first_error_test()
{
	t_token	*tokens;

	tokens = ft_newtoken("|");
	tokens->type = T_PIPE;
	ft_tokenadd_back(&tokens, ft_newtoken("-l"));
	ft_lstlast(tokens)->type = T_WORD;
	ft_tokenadd_back(&tokens, ft_newtoken("-m"));
	ft_lstlast(tokens)->type = T_WORD;
	ft_tokenadd_back(&tokens, ft_newtoken("-n"));
	ft_lstlast(tokens)->type = T_WORD;
	ft_tokenadd_back(&tokens, ft_newtoken("|"));
	ft_lstlast(tokens)->type = T_PIPE;
	ft_tokenadd_back(&tokens, ft_newtoken("|"));
	ft_lstlast(tokens)->type = T_WORD;
	ft_tokenadd_back(&tokens, ft_newtoken("-e"));	
	ft_lstlast(tokens)->type = T_WORD;
	ft_tokenadd_back(&tokens, ft_newtoken("|"));
	ft_lstlast(tokens)->type = T_PIPE;
	ft_tokenadd_back(&tokens, ft_newtoken("twat"));
	ft_lstlast(tokens)->type = T_WORD;
	ft_tokenadd_back(&tokens, ft_newtoken("-b"));	
	ft_lstlast(tokens)->type = T_WORD;
	return(tokens);
}

t_token	*pipe_only_test()
{
	t_token	*tokens;

	tokens = ft_newtoken("|");
	tokens->type = T_PIPE;
	return(tokens);
}

t_node *create_cmd_node(t_token *tokens)
{
	t_node *cmd_node;

	cmd_node = (t_node *)malloc(sizeof(t_node));
	cmd_node->type = A_CMD_WORD;
	cmd_node->content = tokens->content;
	cmd_node->left = NULL;
	cmd_node->right = NULL;
	return (cmd_node);
}

t_node *create_pipe_node(t_token *tokens, t_node *root_node)
{
	t_node *pipe_node;

	pipe_node = (t_node *)malloc(sizeof(t_node));
	pipe_node->type = A_PIPE_SEQUENCE;
	pipe_node->content = NULL;
	pipe_node->left = root_node;
	tokens = tokens->next; // this is the pipe token
	pipe_node->right = create_cmd_node(tokens); // the next command should be the right branch of the pipe node
	return (pipe_node);
}

// t_node *parse(t_token *tokens)
// {
// 	t_node *root_node;

// 	root_node = create_cmd_node(tokens);
// 	root_node = create_pipe_node(tokens, root_node);
// }

int main()
{
	t_token	*tokens;

	tokens = multipipe_test();
	// tokens = single_command_test();
	// tokens = double_pipe_error_test();
	// tokens = pipe_first_error_test();
	// tokens = pipe_only_test();
	pipeline(tokens);

	return (0);
}

// // implements the following line of the grammar: <cmd>		::= T_WORD {T_WORD}
// // create_cmd_node(tokens);
// t_token *cmd(t_token *token)
// {
// 	if (token->type == T_WORD)
// 	{
		
// 		printf("%s ", token->content);
// 		if (!token->next)
// 			return(NULL);
// 		return(cmd(token->next));
// 	}
// 	return(token);
// }

// // implements the following line of the grammar: <pipeline>	::= <cmd> {T_PIPE <cmd>}
// // create_pipe_node(tokens, root_node);
// void	pipeline(t_token *token)
// {
// 	// if (token->type != T_WORD)
// 	// {
// 	// 	printf("syntax-error-pipe\n");
// 	// 	return ;	
// 	// }
// 	token = cmd(token); // undefined behaviour
// 	printf("\n");
// 	if (token == NULL)
// 		return ;
// 	if (token->type == T_PIPE)
// 		pipeline(token->next);
// 	else
// 		printf("syntax-error-pipe\n");
// }
