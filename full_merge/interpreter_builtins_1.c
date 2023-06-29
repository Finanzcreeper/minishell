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
		g_exitstatus = 127;
		return ;
	}
	print_newline = true;
	if (ft_strncmp(elements[0], "-n", ft_strlen(elements[0])) == 0)
	{
		print_newline = false;
		elements++;
	}
	i = 0;
	while (elements[i])
	{
		if (i != 0)
			ft_printf(" ");
		ft_printf("%s", elements[i]);
		i++;
	}
	if (print_newline)
		ft_printf("\n");
	g_exitstatus = 0;
}

// cd (with only a relative or absolute path)
// change current working directory
void	builtin_cd_absolute(char *path)
{
	int	status;

	status = chdir(path);
	if (status != 0)
	{
		ft_printf("bash: cd: %s: No such file or directory\n", path);
		g_exitstatus = 127;
		return ;
	}
	g_exitstatus = 0;
	return ;
}

// concats buf_cwd, slash and path
void	builtin_cd_relative(char *path)
{
	char	buf_cwd[PATH_MAX];
	char	*relative_path;
	int		status;

	if (getcwd(buf_cwd, sizeof(buf_cwd)) != NULL)
	{ 
		relative_path = ft_calloc(ft_strlen(buf_cwd) + ft_strlen(path) + 2, sizeof(char));
		ft_strlcpy(relative_path, buf_cwd, ft_strlen(buf_cwd) + 1);
		ft_strlcat(relative_path, "/", ft_strlen(relative_path) + 2);
		ft_strlcat(relative_path, path, ft_strlen(relative_path) + ft_strlen(path) + 1);
		printf("%s\n", relative_path);
		status = chdir(relative_path);
		free(relative_path);
		if (status != 0)
		{
			ft_printf("bash: cd: %s: No such file or directory\n", path);
			g_exitstatus = 127;
			return ;
		}
		g_exitstatus = 0;
		return ;
	}
	perror("getcwd() error");
	g_exitstatus = 127;
	return ;
}

void	builtin_cd(int num_elements, char **elements)
{
	char	*path;

	if (num_elements > 1)
	{
		ft_printf("cd: too many arguments\n");
		g_exitstatus = 127;
		return ;
	}
	if (num_elements == 0)
	{
		chdir("~");
		g_exitstatus = 0;
		return ;
	}
	if (num_elements == 1)
	{
		path = elements[0];
		if (path[0] == '/')
			builtin_cd_absolute(path);
		else
			builtin_cd_relative(path);
	}
}
