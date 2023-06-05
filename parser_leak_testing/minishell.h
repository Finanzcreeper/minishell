#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <string.h>
# include <signal.h>
# include <limits.h>
# include <ctype.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/ioctl.h>
# include <stdbool.h>
# include "../libft/libft.h"

# define STDIN_FD 0
# define STDOUT_FD 1
# define STDERR_FD 2

# define ERR_CMD "Command Not Found"
# define ERR_FORK "Fork error!"
# define ERR_EXEC "Execution error!"

# define ROOT -2

typedef enum e_token_type
{
	T_WORD = 0,
	T_PIPE = 1,
	T_REFROM = 2,
	T_RETO = 3,
	T_REFROM_HEREDOC = 4,
	T_RETO_APPEND = 5
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
	t_list			*command_elements;
	char			*infile;
	char			*outfile;
}					t_node;

typedef struct s_seperate_arguments_into_nodes
{
	char	*substring;
	int		c;
	int		i;
	int		j;
	int		k;
}t_sain;

typedef struct s_rule
{
	char			*lhs;
	char			**rhs;
	struct s_rule	*next_rule;
}t_rule;

t_token	*ms_tokenizer(char *line);
bool	parse__pipeline(t_token **token, t_node ***ast_head);
void	print_ast(t_node **ast);
t_token	*ft_newtoken(void *content);
void	ft_tokenadd_back(t_token **lst, t_token *new);
void	visit_and_execute(t_node *node, char **env);
void	print_tokens(t_token *token);
char	*tokentype_lookup(int type_num);
void	traverse_ast(t_node *root, char **env);

// interpreter builtins
int		builtin_cd(int num_args, char **args);
void	builtin_echo(int num_args, char **args);
int		builtin_export(int num_args, char **args, char **envp);
int		builtin_unset(char **args, char **envp);
char	**remove_key_from_envp(char **envp, char *key_to_remove);
int		builtin_pwd(void);
void	run_builtin(char *command, int num_args, char *args[], char **envp);

// nicos stuff
t_defs	make_defs(void);
void	token_add_back(t_token **token, t_token *new);
t_token	*new_token(char *content, int type);
t_token	*lexer(char *in, char **envp);

t_defs	make_defs(void);
void	token_add_back(t_token **token, t_token *new);
t_token	*new_token(char *content, int type);
void	pushcurrentsub(t_sain *sain, char *string, t_token **list, t_defs defs);
int		search_dollar(t_token *t, char **env, char *searchterm);
void	fuck_norminete(t_sain *sain, t_defs defs, t_token **list, char *string);
int		is_current_delim(t_defs defs, char *string);

// Quote Handling
void	quote_handler(t_sain *sain, char *string, t_token **list, t_defs defs);
void	single_quoter(t_sain *sain, char *string, t_token **list, t_defs defs);
void	double_quoter(t_sain *sain, char *string, t_token **list, t_defs defs);

// Dollar handling
void	expand_dollars(t_token **list, char **env);
int		is_surrounded_by(t_token *t, char a);
void	strip_quotes(t_token *t, char a); //quote handling exeption
int		dollar_spotted(t_token *t, char **env, int c);
char	*make_before(t_token *t, int c);
char	*make_searched(t_token *t, int *c);
char	*make_after(t_token *t, int c);
int		match_searched(char **env, char *searched);
void	not_found(t_token *t, char *before, char *after, char *searched);
void	found(t_token *t, char *before, char *after, char *searched);

#endif