/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstsize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbooth <gbooth@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/20 21:07:50 by gbooth            #+#    #+#             */
/*   Updated: 2022/12/13 15:14:07 by gbooth           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

static int	ft_count_lstsize(t_list *list, int c)
{
	if (list->next)
		return (ft_count_lstsize(list->next, c + 1));
	else
		return (c + 1);
}

int	ft_lstsize(t_list *lst)
{
	if (lst)
		return (ft_count_lstsize(lst, 0));
	return (0);
}
