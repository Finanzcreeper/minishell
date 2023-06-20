#!/bin/bash

executable_path="./minishell"
verbose=false

compare_with_executable() {
    command_argument="$1"
    expected_output="$2"

    executable_output=$(eval "$executable_path \"$command_argument\"" 2>&1)

    if [ "$verbose" = true ]; then
        echo "Expected Output:"
        echo "$expected_output"
        echo "----------------------------------------------------------"
        echo "Executable Output:"
        echo "$executable_output"
    fi

    if [ "$executable_output" = "$expected_output" ]; then
        echo -e "\e[32mPASS\e[0m"
    else
        echo -e "\e[31mFAIL\e[0m"
    fi
}

# Check if the verbose flag is provided
if [ "$1" = "-v" ]; then
    verbose=true
    shift
fi

# Check if both command and expected output are provided as arguments
if [ $# -eq 2 ]; then
    compare_with_executable "$1" "$2"
else
    command_argument="$1"
    command_output=$(eval "$command_argument" 2>&1)
    executable_output=$(eval "$executable_path \"$command_argument\"" 2>&1)

    if [ "$verbose" = true ]; then
        echo "Bash Command Output:"
        echo "$command_output"
        echo "----------------------------------------------------------"
        echo "Executable Output:"
        echo "$executable_output"
    fi

    diff_output=$(diff <(echo "$command_output") <(echo "$executable_output"))

    if [ -z "$diff_output" ]; then
        echo -e "\e[32mPASS\e[0m"
    else
        echo -e "\e[31mFAIL\e[0m\nDifferences found:"
        echo "----------------------------------------------------------"
        echo "$diff_output"
    fi
fi

echo "=========================================================="
