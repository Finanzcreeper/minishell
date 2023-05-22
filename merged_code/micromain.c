// TODO: problem in visit_and_execute function: after any command, whether succesful or not, control returns correctly to parent process but readline reads an empty line - readline appears to read output from commands as if it is typed input
// TODO: add redirections to grammar

// Q: exit when?
// Q: how/when is get_next_line used? on output of readline?

// cc -Wall -Werror -Wextra -Ilibft -lreadline microlexer.c microparser.c microinterpreter.c micromain.c libft/ft_strncmp.c libft/ft_strjoin.c libft/ft_split.c libft/ft_substr.c libft/ft_strlen.c

// clear; cc -Wall -Werror -Wextra -Ilibft -lreadline microlexer.c microparser.c microinterpreter.c micromain.c libft/ft_strncmp.c libft/ft_strjoin.c libft/ft_split.c libft/ft_substr.c libft/ft_strlen.c && valgrind --leak-check=full ./a.out

// clear; cc -Wall -Werror -Wextra -Ilibft -lreadline microlexer.c microparser.c microinterpreter.c micromain.c libft/ft_strncmp.c libft/ft_strjoin.c libft/ft_split.c libft/ft_substr.c libft/ft_strlen.c && ./a.out

#include "merge.h"

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
	// t_token	*tokens_head;
	// t_node *ast;
	char *line;

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
				tokens = lexer(line);
				print_tokens(tokens);
				// tokens_head = tokens;
				// ast = parse_pipeline(tokens);
				// visit_and_execute(ast, envp);
			}
		}
		free(line);
	}
	// free_tokens(tokens_head);
	return (0);
}
