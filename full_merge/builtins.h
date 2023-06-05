#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <ctype.h>
#include "libft.h"

int	builtin_cd(int num_args, char **args);
void	builtin_echo(int num_args, char **args);
int	builtin_export(int num_args, char **args, char **envp);
int	builtin_unset(char **args, char **envp);
char	**remove_key_from_envp(char **envp, char *key_to_remove);
int builtin_pwd();
void run_builtin(char *command, int num_args, char *args[], char **envp);
