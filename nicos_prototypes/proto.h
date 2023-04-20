/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   proto.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nreher <nreher@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/20 08:42:24 by nreher            #+#    #+#             */
/*   Updated: 2023/04/20 14:31:19 by nreher           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PROTO_H
# define PROTO_H

# include "../Libft/libft.h"

typedef enum e_node_type
{
	T_WORD,
	T_METACHAR,
	T_END	
}t_node_type;

typedef struct s_defs
{
	char	blanks[3];
	char	*metachars[6];
}t_defs;

typedef struct s_token
{
	char			*content;
	int				type;
	struct s_token	*next;
}t_token;

char	**ft_split2(char const *a1, char *delim);

#endif
