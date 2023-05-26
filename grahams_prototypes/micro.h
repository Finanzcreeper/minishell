#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/ioctl.h>
#include <stdbool.h>
#include "libft.h"

#define STDIN_FD 0
#define STDOUT_FD 1
#define STDERR_FD 2

#define ERR_CMD "command not found"
#define ERR_FORK "Fork error!"

#define ROOT -2

typedef enum e_token_types
{
	T_WORD,
	T_REFROM,
	T_RETO,
	T_PIPE	
}					t_token_types;

typedef struct s_token
{
	t_token_types	type;
	char			*content;
	struct s_token	*next;
}					t_token;

typedef enum e_node_types
{
	N_CMD,
	N_PIPE,
}					e_node_types;

typedef struct s_node
{
	int				type;
	struct s_node	*left;
	struct s_node	*right;
	t_list			*cmd_elements;
	char			*infile;
	char			*outfile;
}					t_node;

t_token	*ms_tokenizer(char *line);
bool	parse__pipeline(t_token **token, t_node **ast_root);
void	print_ast(t_node *ast);
t_token	*ft_newtoken(void *content);
void	ft_tokenadd_back(t_token **lst, t_token *new);
void 	visit_and_execute(t_node *node, char **env);
void	print_tokens(t_token *token);
char	*tokentype_lookup(int type_num);
