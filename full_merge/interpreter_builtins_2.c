#include "minishell.h"

// pwd (with no options)
// print the path of the working directory, starting from the root
int	builtin_pwd(void)
{
	char	buf_cwd[PATH_MAX];

	if (getcwd(buf_cwd, sizeof(buf_cwd)) != NULL)
		printf("%s\n", buf_cwd);
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
void	builtin_export_no_args(char **envp)
{
	int		c;
	int		i;
	int		j;
	char	*temp;

	envp = remove_key_from_envp(envp, "_");
	c = 0;
	while (envp[c])
		c++;
	i = 0;
	while (i < c)
	{
		j = 0;
		while (j < c - 1 - i)
		{
			printf("%s\n", envp[j]);
			if (ft_strncmp(envp[j], envp[j + 1], 1) > 0) // which one to ft_strlen? the longest?
			{				
				temp = envp[j];
				envp[j] = envp[j + 1];
				envp[j + 1] = temp;
			}
			j++;
		}
		i++;
	}
}

void builtin_export_args(char **args, char **envp)
{
	int		c;
	int		i;
	int		j;
	char	**new_envp;

	i = 0;
	while (args[i])
	{
		c = 0;
		if (ft_memchr(args[i], '=', ft_strlen(args[i])) != NULL)
		{
			while (envp[c])
				c++;
			new_envp = malloc((c + 1) * sizeof(char *));
			j = 0;
			while (j < c)
			{
				new_envp[j] = envp[j];
				j++;
			}
			new_envp[c] = args[i];
		}
		i++;
	}
}

int	builtin_export(int num_args, char **args, char **envp)
{
	if (num_args == 0)
	{
		builtin_export_no_args(envp);
		return (0);
	}
	builtin_export_args(args, envp);
	return (0);
}

// unset (with no options)
// remove item from envp by name
// NOTE: unsetting a not-previously-set variable does not abort or give an error
int	builtin_unset(char **args, char **envp)
{
	int	i;

	i = 0;
	while (args[i])
	{
		envp = remove_key_from_envp(envp, args[i]);
		i++;
	}
	return (0);
}
