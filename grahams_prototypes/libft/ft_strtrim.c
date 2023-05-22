/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbooth <gbooth@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/20 23:20:05 by gbooth            #+#    #+#             */
/*   Updated: 2022/12/13 15:12:19 by gbooth           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	determine_start(char const *s1, char const *set)
{
	size_t		start;	
	int			i;
	bool		in_set;

	start = 0;
	while (s1[start])
	{
		i = 0;
		in_set = false;
		while (set[i])
		{
			if (s1[start] == set[i])
			{
				in_set = true;
				start++;
			}
			i++;
		}
		if (!in_set)
			break ;
	}
	return (start);
}

static int	determine_end(char const *s1, char const *set)
{
	int		end;
	int		i;
	bool	in_set;

	end = ft_strlen(s1) - 1;
	while (s1[end])
	{
		i = 0;
		in_set = false;
		while (set[i])
		{
			if (s1[end] == set[i])
			{
				in_set = true;
				end--;
			}
			i++;
		}
		if (!in_set)
			break ;
	}
	return (end);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	start;
	size_t	len;
	char	*ret;
	size_t	i;

	start = determine_start(s1, set);
	if (start == ft_strlen(s1))
		len = 0;
	else
	{
		len = determine_end(s1, set) + 1 - start;
	}
	ret = malloc(sizeof(char) * len + 1);
	if (!ret)
		return (NULL);
	i = 0;
	while (i < len)
	{
		ret[i] = s1[i + start];
		i++;
	}
	ret[i] = '\0';
	return (ret);
}
