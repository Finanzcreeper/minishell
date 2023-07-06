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

char *get_string_before_equals(char *str)
{
	char	*pre_eq;
	char	*end;

	pre_eq = ft_strdup(str);
	end = ft_strchr(pre_eq, '=');
	*end = '\0';
	return (pre_eq);
}

int	ll(char *str1, char *str2)
{
	if (ft_strlen(str1) > ft_strlen(str2))
		return (ft_strlen(str1));
	else
		return (ft_strlen(str2));
}

// extract key from env line (all chars before first equals sign)
// used by 'export' and 'unset'
// approach here works but maybe better to
// allocate a new block of memory and copy across
char	**remove_key_from_env(char **env, char *key_to_rm)
{
	int		j;
	char	*line_key;

	j = 0;
	while (env[j])
	{
		line_key = get_string_before_equals(env[j]);
		if (ft_strncmp(key_to_rm, line_key, ll(key_to_rm, line_key)) == 0)
		{
			free(env[j]);
			free(line_key);
			break ;
		}
		free(line_key);
		j++;
	}
	while (env[j] != NULL)
	{
		if (env[j + 1] != NULL)
			env[j] = env[j + 1];
		else
			env[j] = NULL;
		j++;
	}
	return (env);
}
