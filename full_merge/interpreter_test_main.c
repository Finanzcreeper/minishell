
#include "minishell.h"

int	main(int argc, char *argv[], char *env[])
{
	t_node	*_ast;

	_ast = ft_calloc(3, sizeof(t_node));
	_ast[0].type = N_PIPE;
	_ast[1].type = N_CMD;
	_ast[2].type = N_CMD;
	_ast[0].left = &_ast[1];
	_ast[0].right = &_ast[2];
	_ast[1].cmd_elements = ft_lstnew("ls");
	_ast[2].cmd_elements = ft_lstnew("ls");

	visit_and_execute(_ast, env);
	free(_ast);
}
