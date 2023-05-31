/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstlast.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbooth <gbooth@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/20 21:07:50 by gbooth            #+#    #+#             */
/*   Updated: 2022/12/13 15:15:33 by gbooth           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

static	t_list	*ft_lstfindlast(t_list *lst)
{
	if (!lst->next)
		return (lst);
	return (ft_lstfindlast(lst->next));
}

t_list	*ft_lstlast(t_list *lst)
{
	if (lst)
		return (ft_lstfindlast(lst));
	return (NULL);
}
