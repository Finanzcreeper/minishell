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
		g_exitstatus = 127;
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
void	builtin_export_no_args(char **env)
{
	int		c;

	env = remove_key_from_env(env, "_");
	c = 0;
	while (env[c])
		c++;
	bubble_sort_env(env, c);
	format_export_for_display(env, c);
}

char	**builtin_export_args(char **args, char **env)
{
	int		c;
	int		i;
	int		j;
	char	**new_env;

	i = 0;
	while (args[i] != NULL)
	{
		c = 0;
		if (ft_strchr(args[i], '='))
		{
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
		}
		i++;
	}
	return (new_env);
}

char	**builtin_export(char **args, char **env)
{
	int		num_args;
	char	**tempnv;

	args++;
	num_args = 0;
	while (args[num_args])
		num_args++;
	if (num_args == 0)
	{
		builtin_export_no_args(env);
		g_exitstatus = 0;
		return (env);
	}
	tempnv = env;
	env = builtin_export_args(args, env);
	free(tempnv);
	g_exitstatus = 0;
	return (env);
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
	exit(g_exitstatus);
}
