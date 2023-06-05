// clear; cc -Wall -Werror -Wextra -Ilibft builtins1.c builtins2.c builtins3.c builtins_tests.c libft/ft_substr.c libft/ft_strlen.c libft/ft_strchr.c libft/ft_strncmp.c libft/ft_strlcat.c libft/ft_strlcpy.c libft/ft_memchr.c libft/ft_split.c && ./a.out

// TODO: consider removing "getcwd() error" (does it ever happen?)
// NOTE: PATH_MAX = num chars allowed in a full pathname
// TODO: remove status = -1 (done, check if everything still works)
// TODO: remove print checks (for testing)
// TODO: check for memory leaks
// TODO: check using length of correct string in ft_strcmp
// NOTE: can assume $'s will have be expanded in previous stage and whitespace removed from start and end
// Q: do we need handle paths with tildes in 'cd' (considered relative?)
// TODO: in export, my version inserts at end of env, but real version inserts two lines prior [LC_CTYPE]
// TODO: find a way to make envp useable by next command (likely minishell will have a struct with an envp attribute)
// NOTE: in env: '_' entry different when running in terminal to running here ('_=/usr/bin/env vs' '_=/Users/grm/Desktop/minishell/src/./a.out')
// TODO: in exit: get exit status of last executed command (doesn't make sense in this context)


#include "builtins.h"

void	run_test(char *argv, char **envp)
{
	int		num_args;
	char	*command;
	char	**args;

	args = ft_split(argv, ' ');
	if (args[0])
	{
		command = args[0];
		args++;
		num_args = 0;
		while (args[num_args])
			num_args++;
		run_builtin(command, num_args, args, envp);
	}
	// free(args);
	return ;
}

void	test_echo_builtin(char **envp)
{
	printf("TESTING 'echo' BUILTIN\n");
	printf("[\"one two three\" with a newline should be printed to the screen]:\n");
	run_test("echo one two three", envp);

	printf("[\"one two three\" without a newline should be printed to the screen]:\n");
	run_test("echo -n one two three", envp);
	
	printf("[control should be returned directly to the prompt]:\n");
	run_test("", envp);
	printf("\n");
}

void	test_cd_builtin(char **envp)
{
	printf("TESTING 'cd' BUILTIN\n");
	printf("[\"cd\" with no args - does nothing - control should return to prompt]:\n");
	run_test("cd", envp);
    printf("current path is: ");
	builtin_pwd();

	printf("[should correctly navigate to existing relative path - also if in quotes]:\n");
	run_test("cd foldertest", envp);
    printf("current path is: ");
	builtin_pwd();

	printf("[should correctly navigates to existing absolute path - also if in quotes]:\n");
	run_test("cd /tmp", envp);

	printf("[absolute path doesn't exist]:\n");
	run_test("cd /test", envp);

	printf("[relative path doesn't exist]:\n");
	run_test("cd unknown_folder", envp);

	printf("[correctly gives \"cd: too many arguments\" error]:\n");
	// NOTE: [real bash prints "cd: string not in pwd: /test"] - WHY?
	run_test("cd /test this", envp);

	printf("\n");
}

void	test_pwd_builtin(char **envp)
{
	printf("TESTING 'pwd' BUILTIN\n");
	printf("[displays path of current directory, followed by newline]:\n");
	run_test("pwd", envp);

	printf("[displays path of current directory, followed by newline (args ignored)]:\n");
	run_test("pwd one two", envp);
	printf("\n");
}

void	test_export_builtin(char **envp)
{	
	// echoes here do not work! can do: env | grep MY_VAR
	printf("TESTING 'export' BUILTIN\n");
	printf("[argument with a single equals is accepted]:\n");
	run_test("export TEST=this", envp);
	run_test("echo $TEST", envp);

	printf("[argument without a single equals is ignored]:\n");
	run_test("export PWD", envp);
	run_test("echo $PWD", envp);

	printf("[argument multiple equals is accepted]:\n");
	run_test("export TESTINGAGAIN=another=wonderful=test", envp);
	run_test("echo $TESTINGAGAIN", envp);

	printf("[all these are accepted]:\n");
	run_test("export TEST1=one TEST2=two TEST3=three", envp);
	run_test("echo $TEST1 $TEST2 $TEST3", envp);
	printf("\n");
}

void	test_unset_builtin(char **envp)
{
	// echoes here do not work!
	printf("TESTING 'unset' BUILTIN\n");
	printf("[single command: should show double newline i.e. one line space]:\n");
	run_test("unset PWD", envp);
	run_test("echo $PWD", envp);
    // builtin_env(0, args, envp); // use this to print env!

	printf("[multiple commands: should show double newline i.e. one line space]:\n");
	run_test("unset PWD HOME USER", envp);
	run_test("echo $PWD $HOME $USER", envp);

	printf("[should show single newline - no error]:\n");
	run_test("unset", envp);
	printf("\n");
}

void	test_env_builtin(char **envp)
{
	printf("TESTING 'env' BUILTIN\n");
	printf("[print list of environment variables]:\n");
	run_test("env", envp);

	printf("[should produce error \"env: ‘unwantedArg’: No such file or directory]\":\n");
	run_test("env unwantedArg unwantedArg2", envp);

	printf("\n");
}

void	test_exit_builtin(char **envp)
{
	(void)envp;
	// no tests yet
}

int	main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
	// test_echo_builtin(envp);
	// test_cd_builtin(envp);
	// test_pwd_builtin(envp);
	// test_export_builtin(envp);
	// test_unset_builtin(envp);
	test_env_builtin(envp);
	// test_exit_builtin(envp);
}

// int	main(int argc, char **argv, char **envp)
// {
// 	char	*command;

// 	command = argv[1];
// 	argc -= 2;
// 	argv += 2;
// 	run_builtin(command, argc, argv, envp);
// 	return (0);
// }

// char *get_key_from_line(char *line)
// {
// 	char *from_equals;
// 	int key_len;
// 	char *key;

// 	from_equals = ft_strchr(line, '=');
// 	key_len = ft_strlen(line) - ft_strlen(from_equals);
// 	key = ft_substr(line, 0, key_len);
// 	return(key);
// }
