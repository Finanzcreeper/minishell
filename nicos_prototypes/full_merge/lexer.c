#include "minishell.h"

void	set_type(t_token	*token, char **metachars, t_token **token_list)
{
	int		c;

	c = 0;
	token->type = T_WORD;
	if (ft_strncmp(token->content, metachars[0]
			, ft_strlen(token->content)) == 0)
		token->type = T_PIPE;
	else if (ft_strncmp(token->content, metachars[1]
			, ft_strlen(token->content)) == 0)
		token->type = T_REFROM;
	else if (ft_strncmp(token->content, metachars[2]
			, ft_strlen(token->content)) == 0)
		token->type = T_RETO;
	else if (ft_strncmp(token->content, metachars[3]
			, ft_strlen(token->content)) == 0)
		token->type = T_REFROM_HEREDOC;
	else if (ft_strncmp(token->content, metachars[4]
			, ft_strlen(token->content)) == 0)
		token->type = T_RETO_APPEND;
	token_add_back(token_list, token);
}

int	is_current_delim(t_defs defs, char *string)
{
	int	i;

	i = 0;
	while (defs.seperators[i] != NULL)
	{
		if (ft_memcmp(string, defs.seperators[i]
				, ft_strlen(defs.seperators[i])) == 0)
			return (ft_strlen(defs.seperators[i]));
		i++;
	}
	return (0);
}

void	pushcurrentsub(t_sain *sain, char *string, t_token **list, t_defs defs)
{
	if (sain->substring[0] != '\0' && sain->substring[0]
		!= ' ' && sain->substring[0] != '	')
		set_type(new_token(sain->substring, 0), defs.metachars, list);
	ft_bzero(sain->substring, ft_strlen(sain->substring));
	sain->k = 0;
	while (sain->k < sain->i && string[sain->c] != '\0')
	{
		sain->substring[sain->k] = string[sain->c];
		sain->c += 1;
		sain->k += 1;
	}
	sain->i = 0;
}

void	seperate_arguments_into_nodes(char *string, t_defs defs, t_token **list)
{
	t_sain	*sain;

	sain = ft_calloc(1, sizeof(t_sain));
	sain->c = 0;
	sain->j = 0;
	sain->k = 0;
	sain->substring = ft_calloc(ft_strlen(string) + 1, sizeof(char));
	while (string[sain->c] != '\0')
	{
		if (string[sain->c] == '"' || string[sain->c] == '\'')
			quote_handler(sain, string, list, defs);
		if (string[sain->c] == '\0')
			break ;
		sain->i = is_current_delim(defs, &string[sain->c]);
		if (sain->i != 0)
			pushcurrentsub(sain, string, list, defs);
		else
			fuck_norminete(sain, defs, list, string);
	}
	pushcurrentsub(sain, string, list, defs);
	free(sain->substring);
	free(sain);
}

t_token	*lexer(char *in, char **env)
{
	t_defs	defs;
	t_token	*token_list;

	token_list = ft_calloc(1, sizeof(t_token));
	defs = make_defs();
	seperate_arguments_into_nodes(in, defs, &token_list);
	expand_dollars(&token_list, env);
	return (token_list);
}
