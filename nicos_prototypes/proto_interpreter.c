/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   proto_interpreter.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nreher <nreher@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/20 16:42:46 by nreher            #+#    #+#             */
/*   Updated: 2023/04/21 10:58:17 by nreher           ###   ########.fr       */
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
		do_pipe(tree);
	}
	else
	{
	}
}

void	operation(t_node *tree)
{
	int	pid;
	int	pipenbr;
	int	*pipefd[2]

	pipenbr = find_pipenbr(tree);	//need to create find_pipenbr
	ft_calloc(pipenbr, sizeof(int *));
	pipefd = make_pipe(pipefd);		//need to create make_pipe
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
