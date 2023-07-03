fix broken heredoc
✅fix broken append
✅cat | cat | ls
redirections of nothing should just create files - but instead segfault
better error messages
✅running "test" as command gives execution error
builtin checker needs to only find full strings (ech 123 executes echo!! problem with strcmp)
some errors should be prefixed with bash

these command have to work : 
{export feer
env | grep feer
echo $feer}

manual tests to double check:
✅env
heredoc functionality
✅cd with relative and abs paths
✅cd with nothing (home dir)
unset COLORTERM USER PWD (doesn't work at all even for one)
export COLORTERM USER PWD (does work)

what would it take to do bonuses?
