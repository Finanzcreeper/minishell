fix broken heredoc
fix broken append
make interpreter norm compliant
check for forbidden functions i.e. fprintf
cat | cat | ls
redirections of nothing should just create files - but instead segfile
better error messages

manual:
echo "env";
echo "export PWD";
echo "echo $PWD"
heredoc functionality
cd with relative and abs paths
cd with minus (up a dir?)
cd with nothing (home dir)
unset COLORTERM USER PWD (doesn't work at all even for one)
export COLORTERM USER PWD (does work)