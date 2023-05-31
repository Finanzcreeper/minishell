/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbooth <gbooth@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/20 23:20:05 by gbooth            #+#    #+#             */
/*   Updated: 2022/11/29 21:33:28 by gbooth           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	char	*s_cpy;
	int		l;

	s_cpy = (char *)s;
	l = 0;
	while (*s_cpy != '\0')
	{
		l++;
		s_cpy++;
	}
	l++;
	while (l--)
	{
		if (*s_cpy == (unsigned char)c)
			return (s_cpy);
		s_cpy--;
	}
	return (NULL);
}
