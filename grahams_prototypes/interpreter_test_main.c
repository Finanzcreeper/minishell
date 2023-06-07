// run from with full_merge

// clear; cc -Wall -Werror -Wextra -Ilibft interpreter.c ../grahams_prototypes/interpreter_test_main.c interpreter_builtins_1.c interpreter_builtins_2.c interpreter_builtins_3.c ../libft/ft_lstnew.c ../libft/ft_lstadd_back.c ../libft/ft_lstlast.c ../libft/ft_split.c ../libft/ft_strjoin.c ../libft/ft_substr.c ../libft/ft_strncmp.c ../libft/ft_strlen.c ../libft/ft_calloc.c ../libft/ft_bzero.c ../libft/ft_strtrim.c ../libft/ft_memset.c ../libft/ft_strchr.c ../libft/ft_strlcpy.c ../libft/ft_strlcat.c ../libft/ft_memchr.c ../libft/get_next_line.c ../libft/get_next_line_utils.c && ./a.out

// EXAMPLE AST:
//		 		P0
// 			/		\
//	 		P1		C2
// 		/		\	| 
// 		P3		C4	wc -l
// 	/		\	|
// C5		C6	sort -r
// |		|
// cat 	grep this

#include "../full_merge/minishell.h"

int main(int argc, char **argv, char **env)
{
	t_node	nodes[7];
	t_list	*command_elements1;
	t_list	*command_elements2;
	t_list	*command_elements3;
	t_list	*command_elements4;

	(void)argc;
	(void)argv;

	command_elements1 = NULL;
	command_elements2 = NULL;
	command_elements3 = NULL;
	command_elements4 = NULL;

	ft_lstadd_back(&command_elements1, ft_lstnew("cat"));
	ft_lstadd_back(&command_elements2, ft_lstnew("grep"));
	ft_lstadd_back(&command_elements2, ft_lstnew("this"));
	ft_lstadd_back(&command_elements3, ft_lstnew("sort"));
	ft_lstadd_back(&command_elements3, ft_lstnew("-r"));
	ft_lstadd_back(&command_elements4, ft_lstnew("wc"));
	ft_lstadd_back(&command_elements4, ft_lstnew("-l"));

	nodes[0].type = N_PIPE;
	nodes[1].type = N_PIPE;
	nodes[2].type = N_CMD;
	nodes[3].type = N_PIPE;
	nodes[4].type = N_CMD;
	nodes[5].type = N_CMD;
	nodes[6].type = N_CMD;

	nodes[0].infile = NULL; // pipe
	nodes[1].infile = NULL; // pipe
	nodes[2].infile = NULL;
	nodes[3].infile = NULL; // pipe
	nodes[4].infile = NULL;
	nodes[5].infile = malloc(7 * sizeof(char));
	strcpy(nodes[5].infile, "infile");
	nodes[6].infile = NULL;

	nodes[0].outfile = NULL; // pipe
	nodes[1].outfile = NULL; // pipe
	nodes[2].outfile = NULL; // pipe
	// nodes[2].outfile = malloc(8 * sizeof(char));
	// strcpy(nodes[2].outfile, "outfile");
	nodes[3].outfile = NULL; // pipe
	nodes[4].outfile = NULL;
	nodes[5].outfile = malloc(8 * sizeof(char));
	strcpy(nodes[5].outfile, "outfile");
	nodes[6].outfile = NULL;

	nodes[5].read_from_heredoc = true;
	nodes[5].limiter = malloc(4 * sizeof(char));
	strcpy(nodes[5].limiter, "END");

	nodes[5].append_when_writing = false;

	nodes[0].left = &nodes[1];
	nodes[0].right = &nodes[2];
	nodes[1].left = &nodes[3];
	nodes[1].right = &nodes[4];
	nodes[3].left = &nodes[5];
	nodes[3].right = &nodes[6];

	nodes[2].left = NULL;
	nodes[2].right = NULL;
	nodes[4].left = NULL;
	nodes[4].right = NULL;
	nodes[5].left = NULL;
	nodes[5].right = NULL;
	nodes[6].left = NULL;
	nodes[6].right = NULL;

	nodes[5].command_elements = command_elements1;
	nodes[6].command_elements = command_elements2;
	nodes[4].command_elements = command_elements3;
	nodes[2].command_elements = command_elements4;

	// traverse_ast(&nodes[5], env); // 1st cmd: cat
	// traverse_ast(&nodes[3], env); // 1st pipe: cat | grep this
	// traverse_ast(&nodes[1], env); // 1st and 2nd pipe : cat | grep this | sort -r
	// traverse_ast(&nodes[0], env); // 1st 2nd and 3rd pipe : cat | grep this | sort -r | wc -l
	
	// traverse_ast(&nodes[6], env); // grep lexer (hangs waiting for input)
	// traverse_ast(&nodes[4], env); // sort -r
	// traverse_ast(&nodes[2], env); // wc -l

	traverse_ast(&nodes[5], env);
}
