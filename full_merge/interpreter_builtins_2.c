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
		exitstatus = 127;
		return ;
	}
	exitstatus = 0;
}

// export (with no options)
// set environment variables for passing to child processes
// NOTE: performs bubble sort 
// NOTE: arguments are variable names without $
// NOTE: no error message even if unsuccessful
// TODO: tidy and break into functions
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
	
	i = 0;
	while (i < c)
	{
		bool posteq = false;
		ft_printf("declare -x ");
		j = 0;
		while (env[i][j])
		{
			char	qu;
			char	ch;

			qu = '\"';
			ch = env[i][j];
			write(STDOUT_FD, &ch, 1);
			if (posteq == false && env[i][j] == '=')
			{
				posteq = true;
				write(STDOUT_FD, &qu, 1);
			}			
			j++;
		}
		i++;
		ft_printf("\"\n");
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

void	builtin_export(int num_args, char **args, char **env)
{
	if (num_args == 0)
	{
		builtin_export_no_args(env);
		exitstatus = 0;
		return ;
	}
	builtin_export_args(args, env);
	exitstatus = 0;
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
	exitstatus = 0;
	return ;
}
