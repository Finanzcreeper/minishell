// microparser v1
// - take in a linked list of tokens, implement simple grammar for a simple command and pipe, return an ast
// cc -Wall -Werror -Wextra -Ilibft microparser.c libft/ft_strncmp.c -o microparser && ./microparser "ls | wc | wc"
// cc -Wall -Werror -Wextra -Ilibft microparser.c libft/ft_strncmp.c -o microparser && valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file=valgrind-out.txt ./microparser "ls | wc | wc"
// IMPLEMENTATION:
// - scans and parses the input text in one forward pass over the text
// - builds up the parse tree incrementally, bottom up, and left to right, without guessing or backtracking
// Uses SHIFT REDUCE approach:
// -- A SHIFT step advances in the input stream by one symbol. That shifted symbol becomes a new single-node parse tree.
// -- A REDUCE step applies a completed grammar rule to some of the recent parse trees, joining them together as one tree with a new root symbol.
// TODO:
// handle syntax errors correctly. most work apart from this:
// FIXED: "wc wc" [now gives syntax error]
// FIXED: "test | test | test | wc wc" [now gives syntax error]
// how to differentiate between returning NULL for syntax error and returning NULL for malloc protection? maybe just exit on non-protected malloc

// in this grammar there are only two correct forms
// 1. cmd_word PIPE cmd_word
// 2. pipe_sequence PIPE cmd_word

// grammar:
// pipe_sequence	: cmd_word 
// 				| pipe_sequence PIPE cmd_word
// 				;
// cmd_word		: WORD
// 				;

// Q: https://unix.stackexchange.com/questions/270275/what-is-the-difference-between-token-identifier-word-and-name

#include "microlexer.c"
#include <stdio.h>
#include <stdlib.h>

typedef enum e_ast_types
{
	A_CMD_WORD,
	A_PIPE_SEQUENCE,
}	t_ast_types;

typedef struct s_node
{
	int				type; // A_CMD_WORD or A_PIPE_SEQUENCE
	struct s_node	*left; // only relevant for A_PIPE_SEQUENCE
	struct s_node	*right; // only relevant for A_PIPE_SEQUENCE
	char			*content; // only relevant for A_CMD
}					t_node;

// recursively frees linked list of tokens (input should be first token in the list)
void	free_tokens(t_token *tokens)
{
	if (tokens)
	{
		// printf("%s\n", tokens->content);
		if (tokens->next)
			free_tokens(tokens->next);
		free(tokens);
	}
}

t_node *create_cmd_node(t_token *tokens)
{
	t_node *cmd_node;

	cmd_node = (t_node *)malloc(sizeof(t_node));
	if (!cmd_node)
		return (NULL);
	cmd_node->type = A_CMD_WORD;
	cmd_node->content = tokens->content;
	cmd_node->left = NULL;
	cmd_node->right = NULL;
	return (cmd_node);
}

t_node *create_pipe_node(t_token *tokens, t_node *root_node)
{
	t_node *pipe_node;

	if (root_node->type != A_CMD_WORD && root_node->type != A_PIPE_SEQUENCE)
		return(NULL);
	if (tokens->next->type != T_WORD)
		return(NULL);
	pipe_node = (t_node *)malloc(sizeof(t_node));
	if (!pipe_node)
		return(NULL);
		// how to differentiate here between syntax error and fail to allocate?
	pipe_node->type = A_PIPE_SEQUENCE;
	pipe_node->content = NULL;
	pipe_node->left = root_node;
	tokens = tokens->next; // this is the pipe token
	pipe_node->right = create_cmd_node(tokens);
	return (pipe_node);
}

t_node *parse(t_token *tokens)
{
	t_node *root_node;

	while(tokens->type != T_END)
	{
		// switch case here?
		if (tokens->type == T_WORD) 
		{
			if (tokens->prev != NULL) // one cmd_word cannot follow another
				return (NULL);
			root_node = create_cmd_node(tokens);
			if (!root_node)
				return (NULL);
			tokens = tokens->next;
		}
		if (tokens->type == T_PIPE)
		{
			root_node = create_pipe_node(tokens, root_node);
			if (!root_node)
				return (NULL);
			tokens = tokens->next->next;
		}
	}
	return (root_node);
}

void print_ast(t_node *ast)
{
	if (ast == NULL)
		return ;
	print_ast(ast->left);
	if (ast->type == A_PIPE_SEQUENCE)
	{
		printf("|\n");
	}		
	print_ast(ast->right);
	if (ast->type == A_CMD_WORD)
	{
		printf("%s\n", ast->content);
	}
}

void free_ast(t_node *ast)
{
	if (ast == NULL)
		return ;
	free_ast(ast->left);
	free_ast(ast->right);
	free(ast);
}

int main(int argc, char **argv)
{
	t_token *tokens;
	t_node *ast_root_node;

	argc--;
	argv++;
	tokens = ms_tokenizer(*argv);
		ast_root_node = parse(tokens);
	free_tokens(tokens);
	if (!ast_root_node)
	{
		printf("syntax error\n");
		return (-1);
	}
	else
	{
		print_ast(ast_root_node);
	}
	free_ast(ast_root_node);
}
