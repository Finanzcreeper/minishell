#!/bin/bash

if [ "$2" ]; then
    echo "too many args"
    exit
fi

echo "
###############################################"
echo "syntax errors:"
echo "###############################################
"
./t.sh $1 "|" "syntax error!"
./t.sh $1 "|" "syntax error!"
./t.sh $1 "| |" "syntax error!"
./t.sh $1 "||" "syntax error!" 
./t.sh $1 "wc |" "syntax error!"
./t.sh $1 "wc | |" "syntax error!"
./t.sh $1 "wc ||" "syntax error!"
./t.sh $1 ">" "syntax error!"
./t.sh $1 "<" "syntax error!"
./t.sh $1 ">>" "syntax error!"
./t.sh $1 "<<" "syntax error!"
./t.sh $1 ">> >" "syntax error!"
./t.sh $1 "< <<" "syntax error!"
./t.sh $1 "< >" "syntax error!"
./t.sh $1 "| <" "syntax error!"

echo "
###############################################"
echo "command not found:"
echo "###############################################
"
./t.sh $1 "blah" "blah: command not found"
./t.sh $1 "ls | notacommand" "notacommand: command not found"
./t.sh $1 "ls | notacommand | wc" "notacommand: command not found
      0       0       0"
#[wc called with no input resulting in 0 0 0]

echo "
###############################################"
echo "single executables (no args, single arg, multiple args)"
echo "###############################################
"
./t.sh $1 "ls"
./t.sh $1 "ls -l -h"
./t.sh $1 "ls -l -a -h"

echo "
###############################################"
echo "with executables (single, multiple incorrect args)"
echo "###############################################
"
./t.sh $1 "ls -fwef"
./t.sh $1 "man fwfeasdf"
./t.sh $1 "rm nothing"

echo "
###############################################"
echo "with executables (single, multiple incorrect and correct args)"
echo "###############################################
"

./t.sh $1 "ls -fwefw -a"
./t.sh $1 "ls -a -fwefw"
./t.sh $1 "ls -fwefw -a -fwef"
./t.sh $1 "ls -ewwg -wgef -a -wef"

echo "
###############################################"
echo "single builtins (no args, args)"
echo "###############################################
"
## see builtin tester file for more pptential tests
## listed in order specified in subject
./t.sh $1 "echo 123"
./t.sh $1 "echo one 2 three";
./t.sh $1 "echo -n one 2 three";
./t.sh $1 "echo"; # in bash we just get a carriage return
./t.sh $1 "echo -n"; # control returns to prompt, no carriage return

./t.sh $1 "cd"; # cd alone in bash just goes to home dir
./t.sh $1 "cd ." # stay in same dir
./t.sh $1 "cd foldertest" "bash: cd: foldertest: No such file or directory";
./t.sh $1 "cd /tmp";
./t.sh $1 "cd /tmp this" "cd: too many arguments";

./t.sh $1 "pwd"; # only possible case
./t.sh $1 "pwd one two"; # arguments should be ignored

./t.sh $1 "unset"; # does nothing control returns to prompt

./t.sh $1 "env unwantedArg unwantedArg2"; # not expected to reproduce bash behaviour - arguments should be ignored or give error
# ./t.sh $1 "exit" # only possible case - exit not yet implemented

echo "
###############################################"
echo "single pipe with executables"
echo "###############################################
"

./t.sh $1 "ls | wc"
./t.sh $1 "ls -l | wc -l -w -c"

echo "
###############################################"
echo "single pipe with executables and builtins together"
echo "###############################################
"
./t.sh $1 "echo 1 2 3 | wc -l"
./t.sh $1 "pwd | wc -c"
./t.sh $1 "env | grep PATH"
./t.sh $1 "env | head -n 5"

echo "
###############################################"
echo "double pipes"
echo "###############################################
"

./t.sh $1 "env | head -n 5 | wc -l"
./t.sh $1 "env | grep p | grep t"
./t.sh $1 "pwd | wc -w | wc -l"
./t.sh $1 "ls | grep t|wc -l"


echo "
###############################################"
echo "redirections"
echo "###############################################
"

# ./t.sh $1 "< infile cat"
# ./t.sh $1 "cat < infile"
./t.sh $1 "echo 1 2 3 > outfile"
./t.sh $1 "> outfile echo 1 2 3"
./t.sh $1 "echo 1 2 3 >> outfile" #[append]
./t.sh $1 ">> outfile echo 1 2 3" #[append]
./t.sh $1 "< infile > outfile"
# ./t.sh $1 "<< END cat >> outfile"
# ./t.sh $1 "<< END > outfile"

echo "
###############################################"
echo "check these manually!"
echo "###############################################
"

echo "env";
echo "export TEST=this && echo $TEST"; # this check is needed to see if the export worked
echo "export PWD";
echo "echo $PWD"; # this check is needed to see if the export worked

echo "unset PWD && echo $PWD"; # this check is needed to see if the export worked
echo "unset PWD HOME USER && echo $PWD $HOME $USER"; # this check is needed to see if the export worked

# ## redirection only


# ## pipes + redirection together

# < infile cat | grep test
# < infile cat | grep test | wc -l
# ls -l | grep a > outfile

# ## big mix ups (later)

# pwd > outfile
