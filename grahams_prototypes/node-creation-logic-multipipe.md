<REDIRECTION>			::=  'T_RETO' 'T_WORD'
							|  'T_REFROM' 'T_WORD'

<SIMPLE-COMMAND-ELEMENT>	::= 'T_WORD'
							|  <REDIRECTION>

<SIMPLE-COMMAND>			::=  <SIMPLE-COMMAND-ELEMENT>
							|  <SIMPLE-COMMAND> <SIMPLE-COMMAND-ELEMENT>

<PIPELINE> 				::=
							<PIPELINE> 'T_PIPE' <PIPELINE>
							|  <SIMPLE-COMMAND>

# Example: "T_WORD T_PIPE T_WORD T_PIPE T_WORD"

- Start with the initial non-terminal <PIPELINE>.

- Input "T_WORD" expands to <PIPELINE> -> <SIMPLE-COMMAND> -> <SIMPLE-COMMAND-ELEMENT> -> 'T_WORD'. Next token.
-- create CMD_NODE with cmd_elements "T_WORD"
-- create PIPE_NODE and link left branch to CMD_NODE

- Match the input "T_PIPE" with the token "T_PIPE." Next token.

- Input "T_WORD" expands to <PIPELINE> -> <SIMPLE-COMMAND> -> <SIMPLE-COMMAND-ELEMENT> -> 'T_WORD'. Next token.

- Match the input "T_PIPE" with the token "T_PIPE." Next token.

- Input "T_WORD" expands to <PIPELINE> -> <SIMPLE-COMMAND> -> <SIMPLE-COMMAND-ELEMENT> -> 'T_WORD'. No more tokens.

- At this point, we have successfully matched the input "T_WORD T_PIPE T_WORD T_PIPE T_WORD" with the grammar rules.

# Example: "T_WORD"
- should create:
        PIPE
    /           \
CMD             NULL
|
ls

- this shows that we need to create a pipe even for just a single T_WORD
- pipe should be created

To parse the input "T_WORD" using the given grammar, we can follow these steps:

- Start with the initial non-terminal <PIPELINE>.

- Since the input starts with "T_WORD," we can expand <PIPELINE> -> <SIMPLE-COMMAND> -> <SIMPLE-COMMAND-ELEMENT> -> 'T_WORD'. No more tokens.

- At this point, we have successfully matched the input "T_WORD" with the grammar rules.