#!/bin/bash

executable_path="./minishell"
verbose=false
mute_passes=false

compare_with_executable() {
    command_argument="$1"
    expected_output="$2"

    executable_output=$(eval "$executable_path \"$command_argument\"" 2>&1)

    if [ "$executable_output" = "$expected_output" ]; then
		if [ $mute_passes = false ]; then
        	echo -e "\e[32mPASS\e[0m "$1
		    if [ "$verbose" = true ]; then
    		    echo "Expected Output:"
    			echo "$expected_output"
    		    echo "----------------------------------------------------------"
    		    echo "Minishell Output:"
    		    echo "$executable_output"
    		fi
			echo "=========================================================="
		fi
	else
        echo -e "\e[31mFAIL\e[0m "$1
		echo "Expected Output:"
    	echo "$expected_output"
    	echo "----------------------------------------------------------"
        echo "Minishell Output:"
        echo "$executable_output"
		echo "=========================================================="
    fi
}

# Check if a flag is provided
if [[ "$1" == *","* ]]; then
	if [[ "$1" == *"v"* ]]; then
	    verbose=true
	fi
	if [[ "$1" == *"m"* ]]; then
	    mute_passes=true
	fi
	shift
fi

# Check if both command and expected output are provided as arguments
if [ $# -eq 2 ]; then
    compare_with_executable "$1" "$2"
else
    command_argument="$1"
    command_output=$(eval "$command_argument" 2>&1)
    executable_output=$($executable_path "$command_argument" 2>&1)

    diff_output=$(diff <(echo "$command_output") <(echo "$executable_output"))

    if [ -z "$diff_output" ]; then
		if [ $mute_passes = false ]; then
        	echo -e "\e[32mPASS\e[0m "$1
    		if [ "$verbose" = true ]; then
        		echo "Bash Output:"
        		echo "$command_output"
        		echo "----------------------------------------------------------"
        		echo "Minishell Output:"
        		echo "$executable_output"
    		fi
			echo "=========================================================="
		fi
	else
        echo -e "\e[31mFAIL\e[0m "$1
        echo "----------------------------------------------------------"
        echo "Bash Output:"
        echo "$command_output"
        echo "----------------------------------------------------------"
        echo "Minishell Output:"
        echo "$executable_output"
		echo "=========================================================="
    fi
fi

