#include "minishell.h"

// SIGINT is for CTRL + C
void	sigint_handler(int sig)
{
	if (sig == SIGINT)
	{
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
}

void	lexparseinterpret_line(char *line, char ***env)
{
	t_token	*tokens;
	t_token	*token_head;
	t_node	*ast_head;

	ast_head = ft_calloc(1, sizeof(t_node));
	token_head = lexer(line, *env);
	tokens = token_head->next;
	if (tokens == NULL)
	{
		free_ast(ast_head);
		lex_freedman(token_head);
		return ;
	}
	if (parse__pipeline(&tokens, &ast_head) == false)
	{
		ft_printf("%s%s", PRG_NAME, ERR_SYNTAX);
		free_ast(ast_head);
		lex_freedman(token_head);
		return ;
	}
	ast_head->top_node = true;
	traverse_ast(ast_head, env);
	free_ast(ast_head);
	lex_freedman(token_head);
	return ;
}

void	readlines(int infd, int outfd, char ***env)
{
	char	*line;

	while (1)
	{
		dup2(infd, STDIN_FD);
		dup2(outfd, STDOUT_FD);
		line = readline(MSG_PROMPT);
		if (line == NULL)
			return ;
		if (ft_strncmp(line, "clear", 5) == 0)
			rl_clear_history();
		if (line[0] != '\0')
		{
			add_history(line);
			lexparseinterpret_line(line, env);
		}
		free(line);
	}
}

char **dup_env(char **env)
{
	int		c;
	int		j;
	char	**new_env;

	c = 0;
	while (env[c] != NULL)
		c++;
	new_env = ft_calloc(c + 2, sizeof(char *));
	j = 0;
	while (j < c)
	{
		new_env[j] = ft_strdup(env[j]);
		j++;
	}
	return (new_env);
}

int	main(int argc, char **argv, char **orig_env)
{
	int		infd;
	int		outfd;
	char	**env;
	int		i;

	env = dup_env(orig_env);
	infd = dup(STDIN_FD);
	outfd = dup(STDOUT_FD);
	if (argc == 2)
	{
		lexparseinterpret_line(argv[1], &env);
		return (0);
	}
	if (argc > 2)
	{
		ft_printf("%s%s", PRG_NAME, ERR_PRG_ARGS);
		return (0);
	}
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
	readlines(infd, outfd, &env);
	i = 0;
	while (env[i] != NULL)
	{
		free(env[i++]);
	}
	free(env);
	return (0);
}
