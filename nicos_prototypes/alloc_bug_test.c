#include "../full_merge/minishell.h"

int	main(void)
{
	t_node	**a;
	t_node	*p;
	t_node	*b;

	a = NULL;
	p = ft_calloc(10, sizeof(t_node));
	a = &p;
	b = ft_calloc(10, sizeof(t_node));
	ft_printf("test\n");
}
