/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interpreter_builtins_2.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbooth <gbooth@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/06 14:43:54 by gbooth            #+#    #+#             */
/*   Updated: 2023/07/10 13:38:34 by gbooth           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// pwd (with no options)
// print the path of the working directory, starting from the root
void	builtin_pwd(void)
{
	char	buf_cwd[PATH_MAX];

	if (getcwd(buf_cwd, sizeof(buf_cwd)) != NULL)
		ft_printf("%s\n", buf_cwd);
	else
	{
		perror(ERR_CWD);
		g_exitstatus = 1;
		exit(g_exitstatus);
	}
	g_exitstatus = 0;
	exit(g_exitstatus);
}

// export (with no options)
// set environment variables for passing to child processes
// NOTE: performs bubble sort 
// NOTE: arguments are variable names without $
// NOTE: no error message even if unsuccessful
void	builtin_export_no_args(int argc, char **args, char **env)
{
	int		c;

	args++;
	if (argc == 0)
	{
		env = remove_key_from_env(env, "_");
		c = 0;
		while (env[c])
			c++;
		bubble_sort_env(env, c);
		format_export_for_display(env, c);
		g_exitstatus = 0;
		exit(g_exitstatus);
	}
}

char	**builtin_export_args(char **args, char **env)
{
	int		num_args;
	int		i;

	args++;
	num_args = 0;
	while (args[num_args])
		num_args++;
	if (num_args != 0)
	{
		i = 0;
		while (args[i] != NULL)
		{
			if (!ft_strchr(args[i], '='))
			{
				g_exitstatus = 0;
				return (env);
			}
			i++;
		}
		return (export_your_mums_args(args, env));
	}
	return (env);
}

// unset (with no options)
// remove item from env by name
// NOTE: unsetting a not-previously-set variable does not abort or give an error
char	**builtin_unset(char **args, char **env)
{
	int	i;

	args++;
	i = 0;
	while (args[i])
	{
		env = remove_key_from_env(env, args[i]);
		i++;
	}
	g_exitstatus = 0;
	return (env);
}
