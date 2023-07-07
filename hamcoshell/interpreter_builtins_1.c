/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interpreter_builtins_1.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nreher <nreher@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/06 14:43:47 by gbooth            #+#    #+#             */
/*   Updated: 2023/07/07 12:32:30 by nreher           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// echo (with option -n)
// display a line of text passed as an argument
void	builtin_echo(int num_elements, char **elements)
{
	bool	print_newline;
	int		i;

	if (num_elements == 0)
	{
		ft_printf("\n");
		g_exitstatus = 1;
		exit(g_exitstatus);
	}
	print_newline = true;
	if (ft_strncmp(elements[0], "-n", ll(elements[0], "-n")) == 0)
	{
		print_newline = false;
		elements++;
	}
	i = 0;
	while (elements[i])
	{
		if (i != 0)
			ft_printf(" ");
		ft_printf("%s", elements[i++]);
	}
	if (print_newline)
		ft_printf("\n");
	exit(g_exitstatus = 0);
}

// cd (with only a relative or absolute path)
// change current working directory
void	builtin_cd_absolute(char *path)
{
	int	status;

	status = chdir(path);
	if (status != 0)
	{
		ft_printf("%s%s%s%s", PRG_NAME, ERR_CD, path, ERR_FILE);
		g_exitstatus = 1;
		return ;
	}
	g_exitstatus = 0;
	return ;
}

// concats cwd, slash and path
void	builtin_cd_relative(char *path)
{
	char	cwd[PATH_MAX];
	char	*rel_pth;
	int		status;

	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		rel_pth = ft_calloc(ft_strlen(cwd) + ft_strlen(path) + 2, sizeof(char));
		ft_strlcpy(rel_pth, cwd, ft_strlen(cwd) + 1);
		ft_strlcat(rel_pth, "/", ft_strlen(rel_pth) + 2);
		ft_strlcat(rel_pth, path, ft_strlen(rel_pth) + ft_strlen(path) + 1);
		status = chdir(rel_pth);
		free(rel_pth);
		if (status != 0)
		{
			ft_printf("%s%s%s%s", PRG_NAME, ERR_CD, path, ERR_FILE);
			g_exitstatus = 1;
			return ;
		}
		g_exitstatus = 0;
		return ;
	}
	perror(ERR_CWD);
	g_exitstatus = 1;
	return ;
}

void	builtin_cd_single_arg(int argc, char **args)
{
	char			*path;
	char			cwd[PATH_MAX];
	char			*ret;

	if (argc == 1)
	{
		g_exitstatus = 0;
		path = args[0];
		if (ft_strncmp(path, ".", ll(path, ".")) == 0)
			return ;
		if (ft_strncmp(path, "..", ll(path, "..")) == 0)
		{
			if (getcwd(cwd, sizeof(cwd)) != NULL)
			{
				ret = ft_strrchr(cwd, '/');
				*(++ret) = '\0';
			}
			chdir(cwd);
			return ;
		}
		if (path[0] == '/')
			builtin_cd_absolute(path);
		else
			builtin_cd_relative(path);
	}
}

char	**builtin_cd(char **cmd_as_array, char **env)
{
	unsigned int	argc;
	int				c;

	cmd_as_array++;
	argc = 0;
	while (cmd_as_array[argc])
		argc++;
	if (argc > 1)
	{
		ft_printf("%s%s%s", PRG_NAME, ERR_CD, ERR_TM_ARGS);
		g_exitstatus = 1;
		return (env);
	}
	if (argc == 0)
	{
		c = -1;
		while (env[++c] != NULL && ft_strncmp(env[c], "HOME", 
				ft_strlen("HOME")) != 0);
		env = set_env_var("OLDPWD=", env);
		chdir(ft_strchr(env[c], '=') + 1);
		env = set_env_var("PWD=", env);
		g_exitstatus = 0;
		return (env);
	}
	builtin_cd_single_arg(argc, cmd_as_array);
	return (env);
}
