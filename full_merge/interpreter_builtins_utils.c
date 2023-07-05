#include "minishell.h"

void	format_export_for_display(char **env, int c)
{
	int		i;
	int		j;
	char	qu;
	bool	posteq;

	qu = '\"';
	i = 0;
	while (i < c)
	{
		posteq = false;
		ft_printf("declare -x ");
		j = 0;
		while (env[i][j])
		{
			write(STDOUT_FD, &(env[i][j]), 1);
			if (posteq == false && env[i][j] == '=')
			{
				write(STDOUT_FD, &qu, 1);
				posteq = true;
			}
			j++;
		}
		ft_printf("\"\n");
		i++;
	}
}

void	bubble_sort_env(char **env, int c)
{
	int		i;
	int		j;
	int		l;
	char	*temp;

	i = 0;
	while (i < c)
	{
		j = 0;
		while (j < c - i - 1)
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
}

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
		free(line_key);
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

char *get_string_before_equals(char *str)
{
	char	*pre_eq;
	char	*end;

	pre_eq = ft_strdup(str);
	end = ft_strchr(pre_eq, '=');
	*end = '\0';
	return (pre_eq);
}
