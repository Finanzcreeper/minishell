/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbooth <gbooth@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/20 23:20:05 by gbooth            #+#    #+#             */
/*   Updated: 2022/12/13 15:12:28 by gbooth           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	char	*s_;
	size_t	i;

	s_ = (char *)s;
	i = 0;
	c = c % 256;
	if (c == 0)
		return (&s_[ft_strlen(s)]);
	while (s_[i])
	{
		if (s_[i] == (char)c)
			return (&s_[i]);
		i++;
	}
	return (NULL);
}
