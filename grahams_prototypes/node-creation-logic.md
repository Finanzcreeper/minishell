ls -l < infile | grep p > outfile

Open Q's?
- under what conditions do we create a command node?
-- 
- under what conditions do we link the right branch of a pipe to a command
-- if ast 

We have an ast which gets passed around the recursive grammar functions

The operations we should perform are:

When should we perform the follow operations using this example:
ls      -l      <           infile      |       grep    p       >       outfile
T_WORD  T_WORD  T_REFROM    T_WORD      T_PIPE  T_WORD  T_WORD  T_RETO  T_WORD

create a command node
token "ls" is of type T_WORD, which is a <SIMPLE-COMMAND-ELEMENT>
add "ls" to back of command_elements (beware, currently NULL)
move to next token

token "-l" is of type T_WORD, which is a <SIMPLE-COMMAND-ELEMENT>
add "-" to back of command_elements
move to next token

token "<" is of type T_REFROM, which is the first element in a <REDIRECTION>
move to next token
token is of type T_WORD, which is the second elememt in a <REDIRECTION>
move to next token
create the redirection (set 'infile' parameter of cmd_node to "infile")

the combination "ls -l < infile" is treated as <SIMPLE-COMMAND>
which is in turn treated as a <PIPELINE>
and this <PIPELINE> represents the first <PIPELINE> in our '<PIPELINE> 'T_PIPE' <PIPELINE>'

token "-l" is of type T_PIPE, which is the second element in a <PIPELINE>
create a pipe
link the left branch of the pipe to our previous command (ast root)

create a command node
token "grep" is of type T_WORD, which is a <SIMPLE-COMMAND-ELEMENT>
add "ls" to back of command_elements (beware, currently NULL)
move to next token

token "p" is of type T_WORD, which is a <SIMPLE-COMMAND-ELEMENT>
add "p" to back of command_elements
move to next token

token ">" is of type T_RETO, which is the first element in a <REDIRECTION>
move to next token
token "outfile" is of type T_WORD, which is the second elememt in a <REDIRECTION>
move to next token
create the redirection (set 'outfile' parameter of cmd_node to "outfile")
- link the right branch of a pipe to a command

##
as soon as we encounter a valid redirection, we create a command node
as each command is found we add it to the back of the lst