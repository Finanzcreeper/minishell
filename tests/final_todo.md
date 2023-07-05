single space or pasted tabs segfault
check exit status is same as bash for builtins especially
echo $? leaks
Try ctrl-C after running a blocking command like cat without arguments or grep “something“. - we get two minishell prompts

heredoc CTRLC doesn't end
heredoc CTRLD segfaults
minishell% echo '' 123
minishell% echo "" 123
(first '' is seen as -n)
nreher@c4b12c5:~$ unset PATH
nreher@c4b12c5:~$ env | grep PATH
bash: grep: No such file or directory
bash: env: No such file or directory
minishell% echo $DOCKER_HOST

"
minishell% 
minishell% echo $LANG
en_US.UTF-8
minishell% echo $COLORTERM

minishell% unset TERM COLORTERM LANG
minishell% echo $LANG
en_US.UTF-8
"

but "unset USER COLORTERM SHLVL" works


cd .. doesn't work when one level down from root

unset PATH
ls

issues with: "<< END cat >outfile"
