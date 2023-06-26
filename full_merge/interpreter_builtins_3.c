#include "minishell.h"

// env (with no options or arguments)
// print a list of environment variables (in order of creation?)
int	builtin_env(int num_args, char **args, char **env)
{
	int	i;

	if (num_args != 0)
	{
		ft_printf("env: ‘%s’: No such file or directory\n", args[0]);
		return (1);
	}
	i = 0;
	while (env[i])
	{
		ft_printf("%s\n", env[i]);
		i++;
	}
	return (0);
}

// exit (with no options)
// causes shell to exit
// NOTE: exit can have one argument, an exit status code
// (if not given it is that of the last executed command)
int	builtin_exit(int num_args, char arg[])
{
	if (num_args > 1)
	{
		ft_printf("bash: exit: too many arguments\n");
		return (-1);
	}
	if (num_args == 0)
	{
		ft_printf("exit\n");
		// free?
		exit(exitstatus);
	}
	if (isdigit(arg[0]) == false)
	{
		ft_printf("bash: exit: %s: numeric argument required\n", arg[0]);
		return (-1);
	}
	return (0);
}

int	builtin_dollar_question()
{
	ft_printf("%i\n", exitstatus);
	return (0);
}

bool	check_for_builtin(char *command)
{
	if ((ft_strncmp(command, "echo", ft_strlen(command)) == 0) ||
		(ft_strncmp(command, "cd", ft_strlen(command)) == 0) ||
		(ft_strncmp(command, "pwd", ft_strlen(command)) == 0) ||
		(ft_strncmp(command, "export", ft_strlen(command)) == 0) ||
		(ft_strncmp(command, "unset", ft_strlen(command)) == 0) ||
		(ft_strncmp(command, "env", ft_strlen(command)) == 0) ||
		(ft_strncmp(command, "exit", ft_strlen(command)) == 0) ||
		(ft_strncmp(command, "$?", ft_strlen(command)) == 0))
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
	else if (ft_strncmp(cmd, "exit", ft_strlen(cmd)) == 0)
		builtin_exit(argc, args[0]);
	else if (ft_strncmp(cmd, "$?", ft_strlen(cmd)) == 0)
		builtin_dollar_question();		
}
