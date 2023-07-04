#include "minishell.h"

// env (with no options or arguments)
// print a list of environment variables (in order of creation?)
void	builtin_env(int num_args, char **args, char **env)
{
	int	i;

	if (num_args != 0)
	{
		ft_printf("%s‘%s’%s", ERR_ENV, args[0], ERR_FILE);
		g_exitstatus = 127;
		exit(g_exitstatus);
	}
	i = 0;
	while (env[i])
	{
		ft_printf("%s\n", env[i]);
		i++;
	}
	g_exitstatus = 0;
	exit(g_exitstatus);
}

void	detect_non_numeric_arg(char **args)
{
	int	i;

	i = 0;
	while (args[0][i])
	{
		if (args[0][i] < '0' || args[0][i] > '9')
		{
			ft_printf("%s%s%s%s", PRG_NAME, ERR_EXIT, args[0], ERR_NONNUM);
			g_exitstatus = 127;
			exit(g_exitstatus);
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
	ft_printf("exit\n");
	if (argc > 1)
	{
		ft_printf("%s%s%s", PRG_NAME, ERR_EXIT, ERR_TM_ARGS);
		free(--args);
		g_exitstatus = 127;
		exit(g_exitstatus);
	}
	if (argc == 1)
	{
		detect_non_numeric_arg(args);
		g_exitstatus = ft_atoi(args[0]);
	}
	if (argc == 0)
		g_exitstatus = 0;
	free(--args);
	exit(g_exitstatus);
}

// TODO: choose whichever is longer!
bool	is_builtin(char *command, char *builtin)
{
	int l;

	l = 0;
	if (ft_strlen(command) > ft_strlen(builtin))
		l = ft_strlen(command);
	else
		l = ft_strlen(builtin);
	if ((ft_strncmp(command, builtin, l) == 0))
		return (true);
	else
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
	if (is_builtin(cmd, "echo"))
		builtin_echo(argc, args);
	else if (is_builtin(cmd, "pwd"))
		builtin_pwd();
	else if (is_builtin(cmd, "unset"))
		builtin_unset(args, env);
	else if (is_builtin(cmd, "env"))
		builtin_env(argc, args, env);
}
