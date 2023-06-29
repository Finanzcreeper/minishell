builtins:
- DONE: export with no args (tidy function)
- ability to unset and export multiple commands
- FIX: char	relative_path[ft_strlen(buf_cwd) + ft_strlen(path) + 2];
- move cd outside child process

check cd .. and cd .
"echo 1 2 3 > outfile" shouldn't print 1 2 3
implement clear

cleanups:
- check for forbidden functions i.e. fprintf
- break out pipe_to_parent into smaller functions
- norminette

manual:
echo "env";
echo "export TEST=this && echo $TEST"
echo "export PWD";
echo "echo $PWD"
echo "unset PWD && echo $PWD"
echo "unset PWD HOME USER && echo $PWD $HOME $USER"
heredoc functionality
various cd shit - does it go where it should?
cd with minus (up a dir?)
cd with nothing (home dir)
