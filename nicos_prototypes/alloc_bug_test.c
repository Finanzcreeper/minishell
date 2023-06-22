#include "../full_merge/minishell.h"

void	test_func(t_node ***ast)
{
	t_node	*b;

	b = ft_calloc(10, sizeof(t_node));
}

void other_test(t_node ***ast)
{
	t_node	*p;

	p = ft_calloc(10, sizeof(t_node));
	p->infile = "TEST";
	*ast = &p;
}

int	main(void)
{
	t_node	***ast;
	int		c;
	int		a;

	c = 10;
	a = 5;
	c = a;
	ast = ft_calloc(1, sizeof(t_node *));
	other_test(ast);
	test_func(ast);
	ft_printf("test\n");
}
