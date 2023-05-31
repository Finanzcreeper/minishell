/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbooth <gbooth@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/20 23:20:05 by gbooth            #+#    #+#             */
/*   Updated: 2022/12/13 15:48:04 by gbooth           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

static char	**allocate_array_of_strings(char const *s, char c, char **a)
{
	int	i;	
	int	num_words;

	i = 0;
	num_words = 0;
	while (s[i])
	{
		if (s[i] != c && (s[i + 1] == c || s[i + 1] == '\0'))
			num_words++;
		i++;
	}
	a = malloc(sizeof (char *) * (num_words + 1));
	if (!a)
		return (NULL);
	a[num_words] = NULL;
	return (a);
}

char	**ft_split(char const *s, char c)
{
	char	**a;
	size_t	i;
	size_t	ai;
	size_t	start;

	a = NULL;
	a = allocate_array_of_strings(s, c, a);
	if (!a)
		return (NULL);
	i = 0;
	ai = 0;
	start = 0;
	while (s[i])
	{
		if (s[i] == c && s[i + 1] != c)
			start = i + 1;
		if (s[i] != c && (s[i + 1] == c || s[i + 1] == '\0'))
		{
			a[ai] = ft_substr(s, start, i - start + 1);
			if (!a[ai++])
				return (NULL);
		}
		i++;
	}
	return (a);
}
