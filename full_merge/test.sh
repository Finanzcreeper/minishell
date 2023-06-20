#!/bin/bash

compare_output() {
    local actual_output="$1"
    local expected_output="$2"

    if [ "$actual_output" = "$expected_output" ]; then
        echo -e "\e[32mPASS\e[0m"
    else
        echo -e "\e[31mFAIL\e[0m"
    fi
    echo "------------------------------------"
}

test()
{
	local command="$1"
	local expected_output="$2"

	echo -e "ACTUAL:"
	actual_output=$(./minishell "$command")
	echo -e "EXPECTED:\n$expected_output"
	compare_output "$actual_output" "$expected_output"
}

## syntax errors
# test "|" "syntax error!"
# test "|" "syntax error!"
# test "| |" "syntax error!"
# test "||" "syntax error!" 
# test "wc |" "syntax error!"
# test "wc | |" "syntax error!"
# test "wc ||" "syntax error!"
# test ">" "syntax error!"
# test "<" "syntax error!"
# test ">>" "syntax error!"
# test "<<" "syntax error!"
# test ">> >" "syntax error!"
# test "< <<" "syntax error!"
# test "< >" "syntax error!"
# test "| <" "syntax error!"

## command not found
#test "blah" "blah: command not found"
#test "ls | notacommand" "not_a_command: command not found"
#test "ls | notacommand | wc"
##[wc called with no input resulting in 0 0 0]

## single executables (no args, single arg, multiple args)
#test "man" man
#test "man" readline
#ls -l -h
#ls -l -a -h

### with executables (single, multiple incorrect args)
# test "ls -z" ls -z
