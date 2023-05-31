/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbooth <gbooth@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/20 23:20:05 by gbooth            #+#    #+#             */
/*   Updated: 2022/12/02 17:56:22 by gbooth           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	convert_ascii_to_integer(char *nptr)
{
	int	i;
	int	result;

	i = 0;
	result = 0;
	while (nptr[i] != '\0')
	{	
		if (!ft_isdigit(nptr[i]))
			break ;
		result = result * 10;
		result = result + nptr[i] - '0';
		i++;
	}
	return (result);
}

int	ft_atoi(const char *nptr)
{
	char	*nptr_;
	int		result;
	bool	is_minus;

	is_minus = false;
	nptr_ = (char *)nptr;
	while (*nptr_ == ' ' || *nptr_ == '\t' || *nptr_ == '\r'
		|| *nptr_ == '\n' || *nptr_ == '\v' || *nptr_ == '\f')
		nptr_++;
	if (*nptr_ == '+' || *nptr_ == '-')
	{
		if (*nptr_ == '-')
			is_minus = true;
		nptr_++;
	}
	result = 0;
	result = convert_ascii_to_integer(nptr_);
	if (is_minus)
		result = result * -1;
	return (result);
}
