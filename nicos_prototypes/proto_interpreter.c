/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   proto_interpreter.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nreher <nreher@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/20 16:42:46 by nreher            #+#    #+#             */
/*   Updated: 2023/04/20 16:58:02 by nreher           ###   ########.fr       */
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
		operation();
	}
}

int	main(void)
{
	traverse()
}
