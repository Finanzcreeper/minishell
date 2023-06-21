#include "minishell.h"

// extract key from env line (all chars before first equals sign)
// used by 'export' and 'unset'
// approach here works but maybe better to
// allocate a new block of memory and copy across
char	**remove_key_from_env(char **env, char *key_to_remove)
{
	int		j;
	bool	bunch_up;
	char	*from_equals;
	int		line_key_len;
	char	*line_key;

	j = 0;
	bunch_up = false;
	while (env[j])
	{	
		from_equals = ft_strchr(env[j], '=');
		line_key_len = ft_strlen(env[j]) - ft_strlen(from_equals);
		line_key = ft_substr(env[j], 0, line_key_len);
		if (ft_strncmp(key_to_remove, line_key, ft_strlen(key_to_remove)) == 0)
			bunch_up = true;
		if (bunch_up == true)
		{
			if (env[j + 1])
				env[j] = env[j + 1];
			else
				env[j] = NULL;
		}
		j++;
	}
	return (env);
}

// echo (with option -n)
// display a line of text passed as an argument
void	builtin_echo(int num_elements, char **elements)
{
	bool	print_newline;
	int		i;

	if (num_elements == 0)
	{
		ft_printf("\n");
		return ;
	}
	print_newline = true;
	if (ft_strncmp(elements[1], "-n", ft_strlen(elements[1])) == 0)
	{
		print_newline = false;
		elements++;
	}
	i = 1;
	while (elements[i])
	{
		if (i != 1)
			ft_printf(" ");
		ft_printf("%s", elements[i]);
		i++;
	}
	if (print_newline)
		ft_printf("\n");
}

// cd (with only a relative or absolute path)
// change current working directory
int	builtin_cd_absolute(char *path)
{
	int	status;

	status = chdir(path);
	if (status != 0)
	{
		ft_printf("bash: cd: %s: No such file or directory\n", path);
		return (1);
	}
	return (0);
}

// concats buf_cwd, slash and path
int	builtin_cd_relative(char *path)
{
	char	buf_cwd[PATH_MAX];
	int		status;

	if (getcwd(buf_cwd, sizeof(buf_cwd)) != NULL)
	{
		char	relative_path[ft_strlen(buf_cwd) + ft_strlen(path) + 2];
		ft_strlcpy(relative_path, buf_cwd, ft_strlen(relative_path));
		ft_strlcat(relative_path, "/", 1);
		ft_strlcat(relative_path, path, ft_strlen(path));
		status = chdir(path);
		if (status != 0)
		{
			ft_printf("bash: cd: %s: No such file or directory\n", path);
			return (1);
		}
		return (0);
	}
	perror("getcwd() error");
	return (1);
}

int	builtin_cd(int num_elements, char **elements)
{
	char	*path;

	if (num_elements > 2)
	{
		ft_printf("cd: too many arguments\n");
		return (1);
	}
	if (num_elements == 1)
	{
		chdir("~");
		return (0);
	}
	path = elements[1];
	if (path[0] == '/')
		return (builtin_cd_absolute(path));
	return (builtin_cd_relative(path));
}
