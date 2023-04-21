/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nreher <nreher@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/20 10:06:07 by nreher            #+#    #+#             */
/*   Updated: 2023/04/21 09:33:47 by nreher           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "proto.h"

int	is_delim(char mod, char *delim)
{
		if (ft_strncmp(mod, delim, ft_strlen(mod)) == 0)
			return (1);
	return (0);
}

static char	**ft_splitter(char *mod, char **out, char *delim)
{
	unsigned int		c;
	int					i;
	int					l;
	char				*tmp;

	c = 0;
	i = 0;
	while (mod[c])
	{
		tmp = mod;
		mod = ft_strtrim(mod, delim);
		free(tmp);
		l = ft_strlen(mod);
		while (is_delim(mod[c], delim) == 0 && mod[c] != '\0')
			c++;
		out[i] = ft_substr(mod, 0, c);
		i++;
		tmp = mod;
		mod = ft_substr(mod, c, l);
		free(tmp);
		c = 0;
	}
	free(mod);
	return (out);
}

static int	ft_lencnt(char *mod, char *delim)
{
	unsigned int		c;
	int					l;

	c = 0;
	l = 0;
	while (mod[c])
	{
		while (is_delim(mod[c], delim) == 1 && mod[c] != '\0')
			c++;
		l++;
		while (is_delim(mod[c], delim) == 0 && mod[c] != '\0')
			c++;
	}
	return (l);
}

char	**ft_split2(char const *a1, char *delim)
{
	int					l;
	char				*mod;
	char				**out;

	if (a1 == NULL)
		return (NULL);
	mod = ft_strtrim((char *)a1, delim);
	if (a1[0] == '\0' || !mod)
	{
		free(mod);
		out = malloc(1 * sizeof(char *));
		if (!out)
			return (NULL);
		out[0] = NULL;
		return (out);
	}
	l = ft_lencnt(mod, delim);
	out = ft_calloc((l + 1), sizeof(char *));
	if (out == NULL)
		return (NULL);
	ft_splitter(mod, out, delim);
	return (out);
}
