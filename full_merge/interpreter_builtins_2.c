/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_additional_functions.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nreher <nreher@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/03 10:38:35 by nreher            #+#    #+#             */
/*   Updated: 2023/06/27 15:27:54 by nreher           ###   ########.fr       */
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
		perror("getcwd() error");
		g_exitstatus = 127;
		return ;
	}
	g_exitstatus = 0;
}

void	format_export_for_display(char **env, int c)
{
	int		i;
	int		j;
	char	qu;
	bool	posteq;

	qu = '\"';
	i = 0;
	while (i < c)
	{
		posteq = false;
		ft_printf("declare -x ");
		j = 0;
		while (env[i][j])
		{
			write(STDOUT_FD, &(env[i][j]), 1);
			if (posteq == false && env[i][j++] == '=')
			{
				write(STDOUT_FD, &qu, 1);
				posteq = true;
			}
			j++;
		}
		ft_printf("\"\n");
		i++;
	}
}

// export (with no options)
// set environment variables for passing to child processes
// NOTE: performs bubble sort 
// NOTE: arguments are variable names without $
// NOTE: no error message even if unsuccessful
void	builtin_export_no_args(char **env)
{
	int		c;
	int		i;
	int		j;
	int		l;
	char	*temp;

	env = remove_key_from_env(env, "_");
	c = 0;
	while (env[c])
		c++;
	i = 0;
	while (i < c)
	{
		j = 0;
		while (j < c - 1 - i)
		{
			if (ft_strlen(env[j]) > ft_strlen(env[j + 1]))
				l = ft_strlen(env[j]);
			else
				l = ft_strlen(env[j + 1]);
			if (ft_strncmp(env[j], env[j + 1], l) > 0)
			{
				temp = env[j];
				env[j] = env[j + 1];
				env[j + 1] = temp;
			}
			j++;
		}
		i++;
	}
	format_export_for_display(env, c);
}

void builtin_export_args(char **args, char **env)
{
	int		c;
	int		i;
	int		j;
	char	**new_env;

	i = 0;
	while (args[i])
	{
		c = 0;
		if (ft_memchr(args[i], '=', ft_strlen(args[i])) != NULL)
		{
			while (env[c])
				c++;
			new_env = malloc((c + 1) * sizeof(char *));
			j = 0;
			while (j < c)
			{
				new_env[j] = env[j];
				j++;
			}
			new_env[c] = args[i];
		}
		i++;
	}
}

void	builtin_export(int num_args, char **args, char **env)
{
	if (num_args == 0)
	{
		builtin_export_no_args(env);
		g_exitstatus = 0;
		return ;
	}
	builtin_export_args(args, env);
	g_exitstatus = 0;
	return ;
}

// unset (with no options)
// remove item from env by name
// NOTE: unsetting a not-previously-set variable does not abort or give an error
void	builtin_unset(char **args, char **env)
{
	int	i;

	i = 0;
	while (args[i])
	{
		env = remove_key_from_env(env, args[i]);
		i++;
	}
	g_exitstatus = 0;
	return ;
}
