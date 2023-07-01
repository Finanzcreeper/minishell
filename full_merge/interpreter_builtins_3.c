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

void	detect_non_numeric_arg(char **args)
{
	int	i;

	i = 0;
	while (args[0][i])
	{
		if (args[0][i] < '0' || args[0][i] > '9')
		{
			ft_printf("bash: exit: %s: number required\n", args[0]);
			exit(127);
		}
		i++;
	}
}

// exit (with no options)
// causes shell to exit
// can have one argument, an exit status code
// (if not given it is that of the last executed command)
void	builtin_exit(char **args)
{
	unsigned int	argc;

	args++;
	argc = 0;
	while (args[argc])
		argc++;
	if (argc > 1)
	{
		ft_printf("bash: exit: too many arguments\n");
		exit(127);
		return ;
	}
	if (argc == 1)
	{
		detect_non_numeric_arg(args);
		g_exitstatus = ft_atoi(args[0]);
	}
	ft_printf("exit\n");
	if (argc == 0)
		g_exitstatus = 0;
	exit(g_exitstatus);
}

bool	check_for_builtin(char *command)
{
	if ((ft_strncmp(command, "echo", ft_strlen(command)) == 0)
		|| (ft_strncmp(command, "pwd", ft_strlen(command)) == 0)
		|| (ft_strncmp(command, "export", ft_strlen(command)) == 0)
		|| (ft_strncmp(command, "unset", ft_strlen(command)) == 0)
		|| (ft_strncmp(command, "env", ft_strlen(command)) == 0))
		return (true);
	return (false);
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
	else if (ft_strncmp(cmd, "pwd", ft_strlen(cmd)) == 0)
		builtin_pwd();
	else if (ft_strncmp(cmd, "export", ft_strlen(cmd)) == 0)
		builtin_export(argc, args, env);
	else if (ft_strncmp(cmd, "unset", ft_strlen(cmd)) == 0)
		builtin_unset(args, env);
	else if (ft_strncmp(cmd, "env", ft_strlen(cmd)) == 0)
		builtin_env(argc, args, env);
}
