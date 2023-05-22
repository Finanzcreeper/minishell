#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <ctype.h>

char **remove_key_from_envp(char **envp, char* key_to_remove);
void builtin_echo(int num_args, char** args);
int builtin_cd(int num_args, char** args);
int builtin_pwd(int num_args);
char *get_key_from_line(char *line);
int builtin_export(int num_args, char** args, char **envp);
int builtin_unset(int num_args, char** args, char **envp);
int builtin_env(int num_args, char **envp);
int builtin_exit(int num_args, char arg[]);
void run_builtin(char *command, int num_args, char *args[], char **envp);
// int main(int argc, char **argv, char **envp);
