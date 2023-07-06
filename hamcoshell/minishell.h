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

# define PRG_NAME "minishell: "
# define MSG_PROMPT "minishell% "
# define ERR_SYNTAX "syntax error\n"
# define ERR_PRG_ARGS "single argument in quotes required e.g \"ls -l | wc -c\"\n"
# define ERR_CMD ": command not found\n"
# define ERR_FORK "fork error\n"
# define ERR_EXEC "execution error\n"
# define ERR_WRITE "error writing to file\n"
# define ERR_HEREDOC "error reading from heredoc file\n"
# define ERR_CD "cd: "
# define ERR_EXIT "exit: "
# define ERR_ENV "env: "
# define ERR_CWD "getcwd() error"
# define ERR_TM_ARGS "too many arguments\n"
# define ERR_FILE ": No such file or directory\n"
# define ERR_TM_ARGS "too many arguments\n"
# define ERR_NONNUM ": numeric argument required\n"

# define ROOT -2

extern int	g_exitstatus;

typedef enum e_token_type
{
	T_WORD = 0,
	T_PIPE = 1,
	T_REFROM = 2,
	T_RETO = 3,
	T_REFROM_HEREDOC = 4,
	T_RETO_APPEND = 5
}	t_token_type;

typedef enum e_node_types
{
	N_CMD,
	N_PIPE,
}	t_node_types;

typedef struct s_defs
{
	char	blanks[3];
	char	*metachars[6];
	char	*seperators[8];
}	t_defs;

typedef struct s_token
{
	char			*content;
	t_token_type	type;
	struct s_token	*next;
}	t_token;

typedef struct s_node
{
	int				type;
	struct s_node	*left;
	struct s_node	*right;
	t_list			*command_elements;
	char			**cmdarr;
	char			*infile;
	char			*outfile;
	int				in_fd;
	int				out_fd;
	bool			read_from_heredoc;
	char			*limiter;
	bool			append_when_writing;
	bool			top_node;
}	t_node;

typedef struct s_seperate_arguments_into_nodes
{
	char			*substring;
	int				c;
	int				i;
	int				j;
	int				k;
}	t_sain;

typedef struct s_rule
{
	char			*lhs;
	char			**rhs;
	struct s_rule	*next_rule;
}	t_rule;

typedef struct s_redirs
{
	char	*infile;
	char	*outfile;
	bool	append_when_writing;
	bool	read_from_heredoc;
	char	*limiter;
}	t_redirs;

void	lex_freedman(t_token *tokens);
void	free_ast(t_node *ast);

t_defs	make_defs(void);
void	token_add_back(t_token **token, t_token *new);
t_token	*new_token(char *content, int type);
void	pushcurrentsub(t_sain *sain, char *string, t_token **list, t_defs defs);
int		search_dollar(t_token *t, char **env, char *searchterm);
void	fuck_norminete(t_sain *sain, t_defs defs, t_token **list, char *string);
int		is_current_delim(t_defs defs, char *string);
t_token	*lexer(char *in, char **env);

void	quote_handler(t_sain *sain, char *string, t_token **list, t_defs defs);
void	single_quoter(t_sain *sain, char *string, t_token **list, t_defs defs);
void	double_quoter(t_sain *sain, char *string, t_token **list, t_defs defs);

void	expand_dollars(t_token **list, char **env);
int		is_surrounded_by(t_token *t, char a);
void	strip_quotes(t_token *t, char a); // quote handling exeption
int		dollar_spotted(t_token *t, char **env, int c);
char	*make_before(t_token *t, int c);
char	*make_searched(t_token *t, int *c);
char	*make_after(t_token *t, int c);
int		match_searched(char **env, char *searched);
int		not_found(t_token *t, char *before, char *after, char *searched);
void	found(t_token *t, char *before, char *after, char *searched);
char	*int_to_string(int in);

t_token	*ms_tokenizer(char *line);
t_token	*ft_newtoken(void *content);
void	ft_tokenadd_back(t_token **lst, t_token *new);
void	visit_and_execute(t_node *node, char **env);
void	print_tokens(t_token *token);
char	*tokentype_lookup(int type_num);
void	traverse_ast(t_node *ast, char ***env);

int		parse__redirection(t_token **token, t_redirs *redirs);
int		parse__simple_command_element(t_token **token,
			t_list **command_elements, t_redirs *redirs);
void	link_next_command_node_into_tree(t_node *ast_head,
			t_node *node_to_link);
bool	parse__simple_command_tail(t_token **token,
			t_node *ast_head, t_list **command_elements, t_redirs *redirs);
bool	parse__simple_command(t_token **token, t_node *ast_head);
bool	parse__pipeline(t_token **token, t_node **ast_head);
bool	parse__pipeline_tail(t_token **token, t_node **ast_head);

bool	is_builtin(char *command, char *builtin);
void	builtin_exit(char **cmd_as_array);
void	builtin_cd(char **cmd_as_array, char **env);
void	builtin_echo(int num_args, char **args);
char	**builtin_export(char **args, char **env);
char	**builtin_unset(char **args, char **env);
char	**remove_key_from_env(char **env, char *key_to_remove);
char	**count_and_copy_over(char **env, char **args, int i);
void	builtin_pwd(void);
void	run_builtin(char **cmd_as_array, char **env);
bool	check_for_builtin(char *command);
void	bubble_sort_env(char **env, int c);
void	format_export_for_display(char **env, int c);
char	*get_string_before_equals(char *str);
int		ll(char *str1, char *str2);

char	**list_to_array(t_list *list_head);
char	*get_path(char **cmd_as_arr, char **env);
void	open_infile(t_node *cmd_node);
void	open_outfile(t_node *cmd_node);
void	close_inout_fds(t_node *cmd_node);
void	make_heredoc(t_node *cmd_node);

void	sigint_handler(int sig);

#endif
