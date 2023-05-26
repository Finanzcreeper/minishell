/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   proto.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nreher <nreher@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/20 08:42:24 by nreher            #+#    #+#             */
/*   Updated: 2023/05/26 16:11:50 by nreher           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PROTO_H
# define PROTO_H

# include "../libft/libft.h"

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

typedef struct s_rule
{
	char			*lhs;
	char			**rhs;
	struct s_rule	*next_rule;
}t_rule;

typedef struct s_seperate_arguments_into_nodes
{
	char	*substring;
	int		c;
	int		i;
	int		j;
	int		k;
}t_sain;

t_defs	make_defs(void);
void	token_add_back(t_token **token, t_token *new);
t_token	*new_token(char *content, int type);
void	pushcurrentsub(t_sain *sain, char *string, t_token **list, t_defs defs);
int		search_dollar(t_token *t, char **env, char *searchterm);
void	fuck_norminete(t_sain *sain, t_defs defs, t_token **list, char *string);
int		is_current_delim(t_defs defs, char *string);

//Quote Handling
void	quote_handler(t_sain *sain, char *string, t_token **list, t_defs defs);
void	single_quoter(t_sain *sain, char *string, t_token **list, t_defs defs);
void	double_quoter(t_sain *sain, char *string, t_token **list, t_defs defs);

//Dollar handling
void	expand_dollars(t_token **list, char **env);
int		is_surrounded_by(t_token *t, char a);
void	strip_quotes(t_token *t, char a); //quote handling exeption
int		dollar_spotted(t_token *t, char **env, int c);
char	*make_before(t_token *t, int c);
char	*make_searched(t_token *t, int *c);
char	*make_after(t_token *t, int c);
int		match_searched(char **env, char *searched);
void	not_found(t_token *t, char *before, char *after, char *searched);
void	found(t_token *t, char *before, char *after, char *searched);
#endif
