// implementation of the following builtins:
// ◦ echo with option -n
// ◦ cd with only a relative or absolute path
// ◦ pwd with no options
// ◦ export with no options
// ◦ unset with no options
// ◦ env with no options or arguments
// ◦ exit with no options

// TODO: convert strcmp to strlcmp to adhere to libft
// NOTE: can assume $'s will have be expanded in previous stage and whitespace removed from start and end
// Q: handle paths with tildes in 'cd' (considered relative?)
// TODO: in export, my version inserts at end of env, but real version inserts two lines prior [LC_CTYPE]
// TODO: find a way to make envp useable by next command (likely minishell will have a struct with an envp attribute)
// NOTE: in env: '_' entry different when running in terminal to running here ('_=/usr/bin/env vs' '_=/Users/grm/Desktop/minishell/src/./a.out')
// TODO: in exit: get exit status of last executed command (doesn't make sense in this context)

char	*ft_substr(char const *s, unsigned int start, unsigned int len);

// approach here works but maybe better to allocate a new block of memory and copy across
char **remove_key_from_envp(char **envp, char* key_to_remove)
{
	int j = 0;
	bool bunch_up = false;
	while(envp[j])
	{	
		// extract key from envp line (all chars before first equals sign)
		char *from_equals = strchr(envp[j], '=');
		int line_key_len = strlen(envp[j]) - strlen(from_equals);
		char *line_key = ft_substr(envp[j], 0, line_key_len);
		if(strcmp(key_to_remove, line_key) == 0)
			bunch_up = true;
		if (bunch_up == true)
		{
			if (envp[j+1])
				envp[j] = envp[j+1];						
			else
			{
				envp[j] = NULL;
			}
		}
		j++;
	}
	return(envp);
}

// NAME: echo
// DEFINTION: display a line of text passed as an argument
// STATUS: done
// TESTS:
// echo one two three
// ["one two three" with a newline should be printed to the screen]
// echo -n one two three
// ["one two three" without a newline should be printed to the screen]
// echo
// [control should be returned directly to the prompt]
void builtin_echo(int num_args, char** args)
{
	if (num_args == 0)
		return;
	bool print_newline = true;
	if (strcmp(args[0], "-n") == 0)
	{
		print_newline = false;
		args++;
	}
	int i = 0;
	while(args[i])
	{
		if (i != 0)
			printf(" ");
		printf("%s", args[i]);
		i++;
	}
	if (print_newline)
		printf("\n");
}

// NAME: cd
// DEFINITION: change current working directory
// STATUS: done
// TESTS:
// $ cd
// [does nothing - control should return to prompt]
// $ cd /Applications
// [correctly navigates to existing absolute path] PASS
// $ cd folder_i_made
// [correctly navigates to existing relative path] PASS
// NOTE: both of the above also work with paths in quotes
// $ cd /test
// [absolute path doesn't exist - should print error "cd: no such file or directory: /test"]
// [correctly gives error but doesn't match error bash gives yet]
// $ cd unknown_folder
// [relative path doesn't exist - should print error "cd: no such file or directory: /test"]
// $ cd /test this
// [correctly gives "cd: too many arguments" error] PASS
// NOTE: [real bash prints "cd: string not in pwd: /test"] - WHY?
// $ cd /test this please
// [should print "pwd: too many arguments"]
int builtin_cd(int num_args, char** args)
{
	if (num_args == 0)
	{
		return 1;
	}	
	if (num_args > 1) // 'cd with only a relative or absolute path'
	{
		printf("cd: too many arguments\n");
		return 1;
	}
	char *path = args[0];
	if (path[0] == '/') // absolute path
	{
		int status = -1;
		status = chdir(path);
		if (status != 0)
		{
			printf("error!\n");
			return 1;
		}
		// see if the current directory actually changed

	}
	else // relative path
	{
		char buf_cwd[PATH_MAX];
		if (getcwd(buf_cwd, sizeof(buf_cwd)) != NULL)
		{
			// concat buf_cwd, slash and path
			char relative_path[strlen(buf_cwd) + strlen(path) + 2];
			strcpy(relative_path, buf_cwd); // TODO: convert to use strlcpy
			strcat(relative_path, "/"); // TODO: convert to use strlcpy
			strcat(relative_path, path); // TODO: convert to use strlcpy
			int status = -1;
			status = chdir(path);
			if (status != 0)
			{
				printf("error!\n");
			}
		} else
		{
			perror("getcwd() error"); // what is perror?!
			return 1;
		}
	}
	// see if the current directory actually changed
	printf("checking path: ");
	builtin_pwd(0);
	return 0;
}

// NAME: pwd
// DEFINTION: prints the path of the working directory, starting from the root
// STATUS: done
// TESTS:
// pwd
// [displays path of current directory, followed by newline] PASS
// pwd one
// [displays error "pwd: too many arguments", followed by newline] PASS
int builtin_pwd(int num_args)
{
	if (num_args != 0)
	{
		printf("pwd: too many arguments\n");
		return 1;
	}
	char buf_cwd[PATH_MAX];
	if (getcwd(buf_cwd, sizeof(buf_cwd)) != NULL)
	{
		printf("%s\n", buf_cwd);
	} else
	{
		perror("getcwd() error"); // does this ever happen? maybe remove
		return 1;
	}
	return 0;
}

char *get_key_from_line(char *line)
{
	char *from_equals = strchr(line, '=');
	int key_len = strlen(line) - strlen(from_equals);
	char *key = ft_substr(line, 0, key_len);
	return(key);
}

// NAME: export
// DEFINTION: Set environment variables for passing to child processes
// STATUS: done
// NOTE: arguments are variable names without $
// NOTE: control returns to prompt whether export successful or not (i.e. no error message)
// TESTS:
// export TEST=this
// [argument with a single equals is accepted]
// export PWD
// [argument without a single equals is ignored]
// export TESTINGAGAIN=another=wonderful=test
// [argument multiple equals is accepted]
// export TEST1=one TEST2=two TEST3=three
// [all these are accepted]
// MYNUM=42;echo $MYNUM;export MYNUM;
// [can't handle the above as yet, if ever]
int builtin_export(int num_args, char** args, char **envp)
{
	if (num_args == 0)
	{
		envp = remove_key_from_envp(envp, "_");
		// count lines
		int c = 0;
		while(envp[c])
			c++;
		// Sort array using the Buuble Sort algorithm
		for(int i=0; i<c; i++)
		{
			for(int j=0; j<c-1-i; j++)
			{
				if(strcmp(envp[j], envp[j+1]) > 0)
				{
					char *temp;					
					temp = envp[j];
					envp[j] = envp[j+1];
					envp[j+1] = temp;
				}
			}
		}
		builtin_env(0, envp); // print check to see if unset worked
		return 0;
	}
	else
	{
		int i = 0;
		while(args[i]) // export may have multiple arguments, perform export on each one
		{
			int c = 0;
			if(memchr(args[i], '=', strlen(args[i])) != NULL)
			{
				while(envp[c])
				{
					c++;
				}
				char** new_envp = malloc((c + 1) * sizeof(char *));
				int j = 0;
				while(j < c)
				{
					new_envp[j] = envp[j];
					j++;
				}
				new_envp[c] = args[i];
				builtin_env(0, new_envp); // print check to see if included				
			}
			i++;
		}
		return 0;
	}
}

// NAME: unset
// DEFINITION: remove item from envp by name
// STATUS: done
// NOTE: unsetting a not-previously-set variable does not abort or give an error
// TESTS:
// unset PWD; echo $PWD
// [single command: should show nothing]
// unset PWD HOME USER; echo $PWD $HOME $USER
// [multiple commands: should show nothing]
// unset
// [with no args should results in error "unset: not enough arguments"]
// FOR LATER (when multiple commands are possible)
// one=test;two=test2;three=test3;echo $one $two $three;unset one two three;echo $one $two $three
// [should result in no output]
int builtin_unset(int num_args, char** args, char **envp)
{
	// printf("builtin_unset\n");
	if (num_args == 0)
	{
		printf("unset: not enough arguments\n");
		return 1;
	}
	else
	{
		int i = 0;
		while(args[i]) // export may have multiple arguments, perform export on each one
		{
			envp = remove_key_from_envp(envp, args[i]);
			printf("after:\n");
			builtin_env(0, envp); // print check to see if unset worked
			i++;
		}
	}
	return 0;
}

// NAME: env
// DEFINITION: print list of environment variables
// STATUS: done
// TESTS:
// env
// [print list of environment variables in order of creation]
// env unwantedArg
// [should return control to shell without an error message]
int builtin_env(int num_args, char **envp)
{
	// printf("builtin_env\n");	
	// print key,value pairs in the environment
	if (num_args != 0)
	{
		return 1; // 1 meaning error in this case
	}
	else
	{
		int i = 0;
		while(envp[i])
		{
			printf("%s\n", envp[i]);
			i++;
		}
	}
	return 0;
}

// NAME: exit
// DEFINITION: exit the shell where it is currently running
// STATUS: done
// exit can have one argument, an exit status code
// if not given is that of the last executed command
// Q: does return 0 make sense after exit?
int builtin_exit(int num_args, char arg[])
{
	if (num_args > 1)
	{
		printf("exit: too many arguments\n");
		return 1;
	}
	else if (num_args == 1)
	// check if arg is an integer, if so and it's under 127 use it as exit status
	{
		int i = 0;
		while(arg[i])
		{
			if(!isdigit(arg[i]))
			{
				printf("ERROR: argument is not a valid integer\n");
				return 1;
			}
			i++;
		}
		int exit_status = atoi(&arg[0]);
		if (exit_status <= 127)
		{
			exit(exit_status);
			return 0;
		}
		else
		{
			printf("ERROR: exit status is not in range (0 - 127)\n");
		}
	}
	else
	{
		// TODO: get exit status of last executed command (doesn't make sense in this context)
		exit(1);
	}
	return 0;
}

void run_builtin(char *command, int num_args, char *args[], char **envp)
{
	// printf("run_builtin\n");
	if (strcmp(command, "echo") == 0)
		builtin_echo(num_args, args);
	else if (strcmp(command, "cd") == 0)
		builtin_cd(num_args, args);
	else if (strcmp(command, "pwd") == 0)
		builtin_pwd(num_args);
	else if (strcmp(command, "export") == 0)
		builtin_export(num_args, args, envp);
	else if (strcmp(command, "unset") == 0)
		builtin_unset(num_args, args, envp);
	else if (strcmp(command, "env") == 0)
		builtin_env(num_args, envp);
	else if (strcmp(command, "exit") == 0)
		builtin_exit(num_args, args[0]);
}

// int main(int argc, char **argv, char **envp)
// {
// 	char* command = argv[1];
// 	argc -= 2; // discount program name and command
// 	argv += 2; // skip past program name and command
// 	run_builtin(command, argc, argv, envp);
// 	return 0;
// }