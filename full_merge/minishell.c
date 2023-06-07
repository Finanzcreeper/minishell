#include "minishell.h"

void	sigint_handler(int sig)
{
	if (sig == SIGINT) // ctrl + C
	{
		// g_status = 130;
		write(1, "\n", 1);
		rl_on_new_line(); // Tell update functions we have moved onto a new (empty) line
		rl_replace_line("", 0); // Replace the contents of rl_line_buffer with text
		rl_on_new_line(); // Tell the update routines that we have moved onto a new (empty) line, usually after ouputting a newline
		rl_redisplay();// Change what’s displayed on the screen to reflect the current contents of rl_line_buffer. 
	}
}

// used to print the elements of individual commands e.g. "ls -l"
void	print_list(t_list *list)
{
	while (list)
	{
		printf("\"%s\" ", (char *)list->content);
		list = list->next;
	}
}

// to print the ast 
void	print_ast(t_node **ast)
{
	if ((*ast) == NULL)
		return ;
	print_ast(&(*ast)->left);
	if ((*ast)->type == N_PIPE)
		printf("| ");
	print_ast(&(*ast)->right);
	if ((*ast)->type == N_CMD)
	{
		print_list((*ast)->command_elements);
	}
}

void	free_tokens(t_token *tokens_head)
{
	if (tokens_head == NULL)
		return ;
	free_tokens(tokens_head->next);
	free(tokens_head);
}

void	lex_freedman(t_token *tokens)
{
	t_token	*temp;

	while (tokens != NULL)
	{
		temp = tokens;
		tokens = tokens->next;
		free(temp->content);
		free(temp);
	}
}

void	free_ast_node(t_node *temp)
{
	if (temp->command_elements != NULL)
	{
		free(temp->command_elements);
		temp->command_elements = NULL;
	}
	if (temp->infile != NULL)
	{
		free(temp->infile);
		temp->infile = NULL;
	}
	if (temp->outfile != NULL)
	{
		free(temp->outfile);
		temp->outfile = NULL;
	}
	if (temp->right != NULL)
	{
		free(temp->right);
		temp->right = NULL;
	}
	if (temp->left != NULL)
	{
		free(temp->left);
		temp->left = NULL;
	}
	free(temp);
}

void	free_ast(t_node *ast)
{
	if (ast == NULL)
		return ;
	if (ast->left != NULL)
	{
		free_ast(ast->left);
		ast->left = NULL;
	}
	if (ast->right != NULL)
	{
		free_ast(ast->right);
		ast->right = NULL;
	}
	free_ast_node(ast);
	return ;
}

int	main(int argc, char **argv, char **envp)
{
	t_token	*tokens;
	t_token	*token_head;
	t_node	**ast_head;
	char	*line;

	// ast_head = malloc(1 * sizeof(t_node *));
	ast_head = NULL;
	(void)argc;
	(void)argv;
	(void)envp;
	signal(SIGINT, sigint_handler); // display new prompt on new line when CTRL + C pressed
	signal(SIGQUIT, SIG_IGN); // override/ignore default behaviour of CTRL + '\'
	while (1)
	{
		line = readline("minishell% ");
		if (!line) // using this to detect CTRL D, which sends EOF (what if line is actually NULL?)
			break ;
		if (ft_strncmp(line, "clear", 5) == 0)
			rl_clear_history();
		else
		{
			if (line[0] != '\0')
			{
				add_history(line);
				token_head = lexer(line, envp);
				tokens = token_head->next;
				print_tokens(tokens);
				parse__pipeline(&tokens, &ast_head);
				// ft_printf("\nPRINTING AST:\n");
				// print_ast(ast_head);
				// ft_printf("\n");
				traverse_ast(*ast_head, envp);
				// free_ast(*ast_head); // issues here!
				ast_head = NULL;
				lex_freedman(token_head);
			}
		}
		free(line);
	}
	// free_ast(ast_root);
	// free_tokens(tokens_head);
	return (0);
}

