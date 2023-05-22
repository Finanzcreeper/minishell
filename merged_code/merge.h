#ifndef MERGE_H
# define MERGE_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <string.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/ioctl.h>
# include "../libft/libft.h"

# define STDIN_FD 0
# define STDOUT_FD 1
# define STDERR_FD 2

# define ERR_CMD "command not found"
# define ERR_FORK "Fork error!"

# define ROOT -2

typedef enum e_token_type
{
	T_WORD = 0,
	T_PIPE = 1,
	T_RE_FROM = 2,
	T_RE_TO = 3,
	T_RE_FROM_HEREDOC = 4,
	T_RE_TO_APPEND = 5
}t_token_type;

typedef enum e_node_types
{
	N_CMD,
	N_PIPE,
}					t_node_types;

typedef struct s_defs
{
	char	blanks[3];
	char	*metachars[6];
	char	*seperators[8];
}t_defs;

typedef struct s_token
{
	char			*content;
	t_token_type	type;
	struct s_token	*next;
}					t_token;

typedef struct s_node
{
	int				type;
	struct s_node	*left;
	struct s_node	*right;
	t_token			*cmd;
	char			*filename_redir_from;
	char			*filename_redir_to;
}					t_node;

typedef struct s_seperate_arguments_into_nodes
{
	char	*substring;
	int		c;
	int		i;
	int		j;
	int		k;
}t_sain;

t_token	*ms_tokenizer(char *line);
t_node	*parse_pipeline(t_token *token);
void	print_ast(t_node *ast);
t_token	*ft_newtoken(void *content);
void	ft_tokenadd_back(t_token **lst, t_token *new);
void 	visit_and_execute(t_node *node, char **env);
void	print_tokens(t_token *token);
char	*tokentype_lookup(int type_num);
//nicos stuff
t_defs	make_defs(void);
void	token_add_back(t_token **token, t_token *new);
t_token	*new_token(char *content, int type);
t_token	*lexer(char *in);
#endif
