/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbooth <gbooth@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/20 23:20:05 by gbooth            #+#    #+#             */
/*   Updated: 2022/12/13 15:25:08 by gbooth           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*make_new_substr(char const *s, unsigned int len)
{
	char			*ret;
	unsigned int	i;

	ret = NULL;
	ret = malloc(sizeof(char) * len + 1);
	if (!ret)
		return (NULL);
	i = 0;
	while (i < len)
	{
		if (s[i] == '\0')
			return (ret);
		ret[i] = s[i];
		i++;
	}
	ret[i] = '\0';
	return (ret);
}

char	*malloc_empty_string(void)
{
	char			*ret;

	ret = malloc(sizeof(char));
	if (!ret)
		return (NULL);
	ret[0] = '\0';
	return (ret);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char			*ret;

	ret = NULL;
	if ((start == UINT_MAX && len == ULONG_MAX) || len == 0)
		return (malloc_empty_string());
	if (len > ft_strlen(s + start))
		len = ft_strlen(s + start);
	while (start--)
	{
		if (*s == '\0')
			return (malloc_empty_string());
		s++;
	}
	ret = make_new_substr(s, len);
	return (ret);
}
