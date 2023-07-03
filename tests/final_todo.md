fix broken heredoc
fix broken append
cat | cat | ls
redirections of nothing should just create files - but instead segfault
better error messages
cd .. to root doesn't work
running "test" as command gives execution error
echo 123 executes echo!! problem with strcmp
some errors should be prefixed with bash

manual tests to double check:
echo "env";
echo "export PWD";
echo "echo $PWD"
heredoc functionality
cd with relative and abs paths
cd with minus (up a dir?)
cd with nothing (home dir)
unset COLORTERM USER PWD (doesn't work at all even for one)
export COLORTERM USER PWD (does work)

what would it take to do bonuses?