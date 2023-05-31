/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbooth <gbooth@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/20 21:34:00 by gbooth            #+#    #+#             */
/*   Updated: 2022/12/13 15:15:27 by gbooth           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	get_length_of_str_as_int(int n)
{
	int		l;

	l = 0;
	if (n == 0)
		return (1);
	if (n < 0)
	{
		n = n * -1;
		l++;
	}
	while (n > 0)
	{
		n = n / 10;
		l++;
	}
	return (l);
}

char	*generate_chars(char *a, int n, int l)
{
	int		start;

	if (n < 0)
	{
		a[0] = '-';
		n = n * -1;
		start = 1;
	}
	else
		start = 0;
	a[l] = '\0';
	l--;
	while (l >= start)
	{
		a[l] = 48 + (n % 10);
		n = n / 10;
		l--;
	}
	return (a);
}

char	*ft_itoa(int n)
{
	int		l;
	char	*a;
	char	*int_min_str;

	if (n == INT_MIN)
	{
		int_min_str = "-2147483648";
		a = malloc(sizeof(char) * ft_strlen(int_min_str) + 1);
		if (!a)
			return (NULL);
		ft_strlcpy(a, int_min_str, ft_strlen(int_min_str) + 1);
		return (a);
	}
	l = get_length_of_str_as_int(n);
	a = malloc(sizeof(char) * l + 1);
	if (!a)
		return (NULL);
	a = generate_chars(a, n, l);
	return (a);
}
