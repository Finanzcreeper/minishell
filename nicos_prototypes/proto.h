/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   proto.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nreher <nreher@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/20 08:42:24 by nreher            #+#    #+#             */
/*   Updated: 2023/04/21 09:28:11 by nreher           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PROTO_H
# define PROTO_H

# include "../Libft/libft.h"

typedef enum e_token_type
{
	T_WORD = 0,
	T_PIPE = 1,
	T_RE_FROM = 2,
	T_RE_TO = 3,
	T_RE_FROM_HEREDOC = 4,
	T_RE_TO_APPEND = 5
}t_token_type;

typedef enum e_node_type
{
	N_COMMAND,
	N_PIPE	
}t_node_type;
typedef struct s_defs
{
	char	blanks[3];
	char	*metachars[6];
	char	*seperators[8];
}t_defs;

typedef struct s_token
{
	char			*content;
	int				type;
	struct s_token	*next;
}t_token;

typedef struct s_node
{
	char			*content;
	int				type;
	struct s_node	*left;
	struct s_node	*right;
}t_node;

char	**ft_split2(char const *a1, char **delim);

#endif
