# goals:

- the result of the ls command is being interpreted line by line as input to minishell because it goes to the standard out

- fix personal access token
- setup debugger (from scratch)
- fix basic interpreting errors
- fix minishell exiting with % after commands
- DONE: reinstate non-interactive mode for testing purposes
- produce a list of test cases
- handle exit status
- fix exit builtin
- check interpreter for leaks

# TEST cases

## misc

"" [enter returns to prompt (no execution)]

## syntax errors
[bash: syntax error near unexpected token '|']

|
| |
||
wc |
wc | |
wc ||
>
<
>>
<<
>> >
< <<
< >
| <

## command not found

blah
ls | notacommand
ls | notacommand | wc [wc called with no input resulting in 0 0 0]

## single executables (no, single, multiple args)

ls
ls -l
ls -l -h
ls -l -a -h

## single builtins (no args, args)
(see builtin tester file for more tests on this)

pwd
env
echo 1
echo 1 2

"echo"
"cd"
"pwd"
"export"
"unset"
"env"
"exit"

## pipes

### with executables (no, single, multiple args)

ls | wc
ls -l | wc -l -w -c
ls | wc
ls | wc

### with builtins

ls -l | grep a | wc -l

### with executables and builtins together

echo 1 2 3 | wc -l
pwd | wc -c
env | grep PATH
env | sort -r | head -n 5
env | head -n 5 | wc -l

## redirection only

< infile cat
cat < infile
echo 1 2 3 > outfile
> outfile echo 1 2 3
echo 1 2 3 >> outfile [append]
>> outfile echo 1 2 3 [append]
<< END cat >> outfile
<< END > outfile

## pipes + redirection together

< infile cat | grep test
< infile cat | grep test | wc -l
ls -l | grep a > outfile

## big mix ups (later)

pwd > outfile