/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interpreter_builtins_utils_2.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbooth <gbooth@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/06 14:44:12 by gbooth            #+#    #+#             */
/*   Updated: 2023/07/06 14:44:13 by gbooth           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**count_and_copy_over(char **env, char **args, int i)
{
	int		c;
	int		j;
	char	**new_env;

	c = 0;
	while (env[c] != NULL)
		c++;
	new_env = ft_calloc(c + 2, sizeof(char *));
	j = 0;
	while (j < c)
	{
		new_env[j] = env[j];
		j++;
	}
	new_env[c] = ft_strdup(args[i]);
	return (new_env);
}
