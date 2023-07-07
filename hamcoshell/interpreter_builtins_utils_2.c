/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interpreter_builtins_utils_2.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nreher <nreher@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/06 14:44:12 by gbooth            #+#    #+#             */
/*   Updated: 2023/07/07 12:38:08 by nreher           ###   ########.fr       */
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

char	**set_env_var(char *key, char **env)
{
	char	**args;
	char	buf_cwd[PATH_MAX];
	char	**nenv;

	args = ft_calloc(sizeof(char *), 3);
	getcwd(buf_cwd, sizeof(buf_cwd));
	args[0] = ft_strdup("export");
	args[1] = ft_strjoin(key, buf_cwd);
	nenv = builtin_export_args(args, env);
	free(args[0]);
	free(args[1]);
	free(args);
	return (nenv);
}
