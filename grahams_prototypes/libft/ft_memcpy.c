/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbooth <gbooth@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/20 21:08:01 by gbooth            #+#    #+#             */
/*   Updated: 2022/12/13 15:14:10 by gbooth           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	const char	*c_src;
	char		*c_dest;

	if (dest == NULL && src == NULL)
		return (NULL);
	c_src = (char *)src;
	c_dest = (char *)dest;
	while (n--)
		*c_dest++ = *c_src++;
	return (dest);
}
