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

// env (with no options or arguments)
// print a list of environment variables (in order of creation?)
void	builtin_env(int num_args, char **args, char **env)
{
	int	i;

	if (num_args != 0)
	{
		ft_printf("env: ‘%s’: No such file or directory\n", args[0]);
		g_exitstatus = 127;
		return ;
	}
	i = 0;
	while (env[i])
	{
		ft_printf("%s\n", env[i]);
		i++;
	}
	g_exitstatus = 0;
}

// exit (with no options)
// causes shell to exit
// can have one argument, an exit status code
// (if not given it is that of the last executed command)
void	builtin_exit(char **cmd_as_array)
{
	char			*cmd;
	char			**args;
	unsigned int	argc;
	int 			i;

	cmd = cmd_as_array[0];
	args = ++cmd_as_array;
	argc = 0;
	while (args[argc])
		argc++;

	if (argc > 1)
	{
		ft_printf("bash: exit: too many arguments\n");
		g_exitstatus = 127;
		return ;
	}
	if (argc == 1)
	{
		i = 0;
		while (args[0][i])
		{
			if (args[0][i] < '0' || args[0][i] > '9')
			{
				ft_printf("bash: exit: %s: numeric argument required\n", args[0]);
				g_exitstatus = 127;
				return ;
			}
			i++;
		}
		g_exitstatus = ft_atoi(args[0]);
	}
	if (argc == 0)
		g_exitstatus = 0;
	ft_printf("exit\n");
	exit(g_exitstatus);
}

bool	check_for_builtin(char *command)
{
	if ((ft_strncmp(command, "echo", ft_strlen(command)) == 0) ||
		(ft_strncmp(command, "cd", ft_strlen(command)) == 0) ||
		(ft_strncmp(command, "pwd", ft_strlen(command)) == 0) ||
		(ft_strncmp(command, "export", ft_strlen(command)) == 0) ||
		(ft_strncmp(command, "unset", ft_strlen(command)) == 0) ||
		(ft_strncmp(command, "env", ft_strlen(command)) == 0))
		return true;
	return false;
}

void	run_builtin(char **cmd_as_array, char **env)
{
	char			*cmd;
	char			**args;
	unsigned int	argc;

	cmd = cmd_as_array[0];
	args = ++cmd_as_array;
	argc = 0;
	while (args[argc])
		argc++;
	if (ft_strncmp(cmd, "echo", ft_strlen(cmd)) == 0)
		builtin_echo(argc, args);
	else if (ft_strncmp(cmd, "cd", ft_strlen(cmd)) == 0)
		builtin_cd(argc, args);
	else if (ft_strncmp(cmd, "pwd", ft_strlen(cmd)) == 0)
		builtin_pwd();
	else if (ft_strncmp(cmd, "export", ft_strlen(cmd)) == 0)
		builtin_export(argc, args, env);
	else if (ft_strncmp(cmd, "unset", ft_strlen(cmd)) == 0)
		builtin_unset(args, env);
	else if (ft_strncmp(cmd, "env", ft_strlen(cmd)) == 0)
		builtin_env(argc, args, env);
}
