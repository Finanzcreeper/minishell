/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbooth <gbooth@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/20 23:20:05 by gbooth            #+#    #+#             */
/*   Updated: 2022/12/08 09:32:45 by gbooth           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*join_strs(char const *s1, char const *s2, uint l1, uint l2)
{
	char			*ret;

	ret = malloc(sizeof(char) * (l1 + l2 + 1));
	if (!ret)
		return (NULL);
	l1 = 0;
	while (s1[l1] != '\0')
	{
		ret[l1] = s1[l1];
		l1++;
	}
	l2 = 0;
	while (s2[l2] != '\0')
	{
		ret[l1 + l2] = s2[l2];
		l2++;
	}
	ret[l1 + l2] = '\0';
	return (ret);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	uint	l1;
	uint	l2;
	char	*ret;

	if (s1 == NULL || s2 == NULL)
		return (NULL);
	l1 = 0;
	while (s1[l1] != '\0')
		l1++;
	l2 = 0;
	while (s2[l2] != '\0')
		l2++;
	ret = join_strs(s1, s2, l1, l2);
	return (ret);
}
