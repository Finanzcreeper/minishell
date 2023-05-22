// microlexer v1
// - take in a string representing a command line and output a linked list of tokens
// cc -Wall -Werror -Wextra -ILibft microlexer.c Libft/ft_strncmp.c -o microlexer && ./microlexer "ls -l"

#include "micro.h"

t_token_types	catego_toketype(char *content)
{
	if (!ft_strncmp(content, "|", 1))
		return (T_PIPE);
	else if (!ft_strncmp(content, "<", 1))
		return (T_REFROM);
	else if (!ft_strncmp(content, ">", 1))
		return (T_RETO);
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
	while (current->next)
	{
		current = current->next;
	}
	current->next = new;
}

t_token	*ft_newtoken(void *content)
{
	t_token		*re;

	re = (t_token *)malloc(sizeof(t_token));
	if (!re)
		return (NULL);
	re->content = content;
	re->type = catego_toketype(content);
	re->next = NULL;
	return (re);
}

int	sep(char s)	//returns if a char is a seperator
{
	if (s == ' ' || s == '>' || s == '<' || s == '|')
		return (1);
	return (0);
}

int	count_sep(char *s, int len)
{
	while (sep(*s) && (*s) != ' ')	//checks if the current pointed at char in s is a Seperator but not a space (' ')
	{
		s++;						//advances the currently pointed at char in s
		len++;						//increases the length of the seperator read [NEVER STOPS!!!]
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
	current = ft_newtoken(arr);
	return (current);
}

/*	Nicos version	*/
t_token	*ms_tokenizer(char *line)
{
	t_token	*all;
	t_token	*current;
	int		len;
	int	c;

	all = NULL;
	c = 0;
	while(line[c] != '\0')
	{
		while(line[c] == ' ' && line[c] != '\0')
			c++;
		len = count_len(&line[c]);
		if (len == 0)
			break ;
		current = check_each(len, &line[c]);
		ft_tokenadd_back(&all, current);
		c += len;
	}
	return (all);
}

char *tokentype_lookup(int type_num)
{
	char *type_str;

	type_str = NULL;
	if (type_num == 0)
		type_str = "T_WORD";
	else if (type_num == 1)
		type_str = "T_REFROM";
	else if (type_num == 2)
		type_str = "T_RETO";
	else if (type_num == 3)
		type_str = "T_PIPE";
	return(type_str);
}
