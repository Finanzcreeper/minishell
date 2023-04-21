/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   proto_interpreter.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nreher <nreher@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/20 16:42:46 by nreher            #+#    #+#             */
/*   Updated: 2023/04/21 09:10:36 by nreher           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "proto.h"

void	traverse(t_node *tree)
{
	if (tree == NULL)
		return ;
	if (tree->type = N_PIPE)
	{
		traverse(tree->left);
		traverse(tree->right);
	}
	else
	{
		operation(tree);
	}
}

void	operation(t_node *tree)
{
	int	pid;

	pid = fork();
	if (pid == 0)
	{

	}
	else
	{
		wait (NULL);
	}
}
/*  >out < test ls  < test1 -la  -> in word word*/
/*   grep  -> word */ 
// dup2(text.txt, STDIN_FILENO);

char	**arg_maker(char *in)
{
	if (ft_strnstr(in, "/", ft_strlen(in)) != NULL)

}

int	main(void)
{
	traverse()
}
