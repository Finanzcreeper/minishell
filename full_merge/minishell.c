// TODO: problem in visit_and_execute function: after any command, whether succesful or not, control returns correctly to parent process but readline reads an empty line - readline appears to read output from commands as if it is typed input

// Q: exit when?
// Q: how/when is get_next_line used? on output of readline?

// clear && cc -Wall -Werror -Wextra -Ilibft -lreadline -g3 microlexer.c microparser.c microinterpreter.c micromain.c libft/ft_strncmp.c libft/ft_strjoin.c libft/ft_split.c libft/ft_substr.c libft/ft_strlen.c libft/ft_lstnew.c libft/ft_lstadd_back.c libft/ft_lstlast.c && ./a.out

#include "minishell.h"

// https://www.geeksforgeeks.org/signals-c-language/
// allowed functions (signals): signal, sigaction, sigemptyset, sigaddset, kill

void sigint_handler(int sig)
{
	if (sig == SIGINT) // ctrl + C
	{
		// g_status = 130;
		write(1, "\n", 1);
		rl_on_new_line(); // Tell update functions we have moved onto a new (empty) line
		rl_replace_line("", 0); // Replace the contents of rl_line_buffer with text
		rl_on_new_line(); // Tell the update routines that we have moved onto a new (empty) line, usually after ouputting a newline
		rl_redisplay(); // Change what’s displayed on the screen to reflect the current contents of rl_line_buffer. 
	}
}

// used to print the elements of individual commands e.g. "ls -l"
void	print_list(t_list *list)
{
	while(list)
	{
		printf("\"%s\" ", (char *)list->content);
		list = list->next;
	}
}

// to print the ast 
void print_ast(t_node *ast)
{
	if (ast == NULL)
		return ;
	print_ast(ast->left);
	if (ast->type == N_PIPE)
		printf("| ");
	print_ast(ast->right);
	if (ast->type == N_CMD)
	{
		print_list(ast->cmd_elements);
	}
}

void free_tokens(t_token *tokens_head)
{
	if (tokens_head == NULL)
		return ;
	free_tokens(tokens_head->next);
	free(tokens_head);
}

int main(int argc, char **argv, char **envp)
{
	t_token	*tokens;
	t_node *pipe_node;
	t_node *cmd_node;
	char *line;

	pipe_node = NULL;
	cmd_node = NULL;
	(void)argc;
	(void)argv;
	(void)envp;
	signal(SIGINT, sigint_handler); // display new prompt on new line when CTRL + C pressed
	signal(SIGQUIT, SIG_IGN); // override/ignore default behaviour of CTRL + '\'
	while (1)
	{
		line = readline("microshell% ");
		if (!line) // using this to detect CTRL D, which sends EOF (what if line is actually NULL?)
			break ;
		if (ft_strncmp(line, "clear", 5) == 0)
			rl_clear_history();
		else
		{
			if (line[0] != '\0')
			{
				add_history(line);
				tokens = lexer(line, envp);
				print_tokens(tokens);
				parse__pipeline(&tokens, &pipe_node, &cmd_node);
				printf("\nPRINTING AST:\n");
				print_ast(pipe_node);
				printf("\n");
				// visit_and_execute(ast, envp);
			}
		}
		free(line);
	}
	// free_ast(ast_root);
	// free_tokens(tokens_head);
	return (0);
}
