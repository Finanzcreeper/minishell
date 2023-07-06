✅ single space or pasted tabs segfault
G - check exit status is same as bash for builtins especially
G - can't seem to handle _ in env names i.e. echo $DOCKER_HOST
G - can't unset some env vars i.e. "unset TERM COLORTERM LANG", "echo $LANG" gives "en_US.UTF-8" but "unset USER COLORTERM SHLVL" works
G - cd .. doesn't work when one level down from root
G - can still do 'ls' after unsetting PATH 

✅ echo $? leaks
Try ctrl-C after running a blocking command like cat without arguments or grep “something“. - we get two minishell prompts

heredoc CTRLC doesn't end
heredoc CTRLD segfaults
minishell% echo '' 123 (first '' is seen as -n) also for ""

nreher@c4b12c5:~$ unset PATH
nreher@c4b12c5:~$ env | grep PATH
bash: grep: No such file or directory
bash: env: No such file or directory

issues with: "<< END cat >outfile"
