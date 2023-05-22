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
