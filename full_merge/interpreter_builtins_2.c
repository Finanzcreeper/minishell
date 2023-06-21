#include "minishell.h"

// pwd (with no options)
// print the path of the working directory, starting from the root
int	builtin_pwd(void)
{
	char	buf_cwd[PATH_MAX];

	if (getcwd(buf_cwd, sizeof(buf_cwd)) != NULL)
		ft_printf("%s\n", buf_cwd);
	else
	{
		perror("getcwd() error");
		return (1);
	}
	return (0);
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
			ft_printf("%s\n", env[j]);
			if (ft_strncmp(env[j], env[j + 1], 1) > 0) // which one to ft_strlen? the longest?
			{				
				temp = env[j];
				env[j] = env[j + 1];
				env[j + 1] = temp;
			}
			j++;
		}
		i++;
	}
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

int	builtin_export(int num_args, char **args, char **env)
{
	if (num_args == 0)
	{
		builtin_export_no_args(env);
		return (0);
	}
	builtin_export_args(args, env);
	return (0);
}

// unset (with no options)
// remove item from env by name
// NOTE: unsetting a not-previously-set variable does not abort or give an error
int	builtin_unset(char **args, char **env)
{
	int	i;

	i = 0;
	while (args[i])
	{
		env = remove_key_from_env(env, args[i]);
		i++;
	}
	return (0);
}
