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
./t.sh $1 "echo 123"

echo "
###############################################"
echo "with executables (single, multiple incorrect args)"
echo "###############################################
"
./t.sh $1 "ls -fwef"
./t.sh $1 "man fwfeasdf"
./t.sh $1 "rm nothing"
