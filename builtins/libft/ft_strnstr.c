/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbooth <gbooth@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/20 23:20:05 by gbooth            #+#    #+#             */
/*   Updated: 2022/12/05 13:50:06 by gbooth           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	char		*big_;
	size_t		i;
	int			j;

	big_ = (char *)big;
	if (len == 0 && little[0])
		return (NULL);
	if (little[0] == '\0')
		return (&big_[0]);
	i = 0;
	while (big_[i] && i < len)
	{
		j = 0;
		while (big_[i + j] == little[j] && i + j < len)
		{
			j++;
			if (little[j] == '\0')
				return (&big_[i]);
		}
		i++;
	}
	return (NULL);
}
