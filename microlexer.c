// microlexer v1
// - take in a line, output a linked list of tokens
// cc -Wall -Werror -Wextra -Ilibft microlexer.c libft/ft_strncmp.c -o microlexer && ./microlexer "ls -l"
// understand stolen tokenisation algorithm!

#include <stdio.h>
#include <stdlib.h>
#include "libft.h"

typedef enum e_token_types
{
	T_END = -2,
	T_WORD = 0,
	T_PIPE	
}	t_token_types;

typedef struct s_token
{
	int				type;
	char			*content;
	struct s_token	*prev;	
	struct s_token	*next;
}					t_token;

t_token_types	catego_toketype(char *content)
{
	if (!content)
		return (T_END);
	if (!ft_strncmp(content, "|", 1))
		return (T_PIPE);
	else
		return (T_WORD);
}

void	ft_tokenadd_back(t_token **lst, t_token *new)
{
	t_token	*current;

	if (!(*lst))
	{
		(*lst) = new;
		return ;
	}
	current = (*lst);
	while (current->next != 0)
	{
		current = current->next;
	}
	current->next = new;
	current->next->prev = current; // enable ability to look back	
}

t_token	*ft_newtoken(void *content)
{
	t_token		*re;

	re = (t_token *)malloc(sizeof(t_token));
	if (!re)
		return (NULL);
	re->content = content;
	re->type = (int)catego_toketype(content);
	re->next = 0;
	return (re);
}

int	sep(char s)
{
	if (s == ' ' || s == '>' || s == '<' || s == '|')
		return (1);
	return (0);
}

int	count_sep(char *s, int len)
{
	while (sep(*s) && (*s) != ' ')
	{
		s++;
		len++;
	}
	return (len);
}

int	count_len(char *s)
{
	int		len;
	int		i;

	len = 0;
	i = -1;
	while (s[++i])
	{
		if (sep(s[i]))
			break ;
		len++;
	}
	if (sep(*s))
		len = count_sep(s, len);
	return (len);
}

t_token	*check_each(int len, char *s)
{
	int		i;
	char	*arr;

	t_token	*current;

	i = 0;
	arr = (char *)malloc((len + 1) * sizeof(char));
	if (!arr)
		return (0);
	while (i < len)
	{		
		arr[i++] = *s;
		s++;
	}
	arr[i] = '\0';
	// printf("ce: %s,  %s\n", s, arr);
	current = ft_newtoken(arr);
	return (current);
}

t_token	*ms_tokenizer(char *line)
{
	t_token	*all;
	t_token	*current;
	int		len;

	all = 0;
	while (*line)
	{
		while (*line == ' ' && *line)
			line++;
		len = count_len(line);
		if (!len)
			break ;
		current = check_each(len, line);
		ft_tokenadd_back(&all, current);
		line = line + len;
	}
	ft_tokenadd_back(&all, ft_newtoken(NULL));
	return (all);
}

char *tokentype_lookup(int type_num)
{
	char *type_str;

	type_str = NULL;
	if (type_num == -2)
		type_str = "T_END";
	else if (type_num == 0)
		type_str = "T_WORD";
	else if (type_num == 1)
		type_str = "T_PIPE";
	return(type_str);
}

// // line = first argument of main (in quotes) e.g. "he >> ho || ha"
// int main(int argc, char **argv)
// {
// 	t_token	*all;
	
// 	argc--;
// 	argv++;
// 	all = ms_tokenizer(*argv);
// 	while(all)
// 	{
// 		printf("content: '%s'\n", all->content);
// 		printf("type: %s\n", tokentype_lookup(all->type));
// 		all=all->next;
// 	}
// }