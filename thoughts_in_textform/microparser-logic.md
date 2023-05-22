iterate over tokens:
1 	ls	T_WORD
2 	|	T_PIPE
3 	wc	T_WORD
4 	|	T_PIPE
5 	wc	T_WORD
6		T_END

STEP 1.
^if token is of type T_WORD
^- create_cmd_node with content of the token ("ls")
^- set this node as root_node
^- advance token

STEP 2.
^if token is of type_T_PIPE
^if existing node type (left of pipe) is not A_CMD_WORD or A_PIPE_SEQUENCE return NULL
^if next token type is not T_WORD return NULL
^- create_pipe_node
^- set left branch to root_node
^- set this node as root_node
^- advance token

STEP 3.
^because token is of type T_WORD
^- advance the token
^- create_cmd_node with content of the token ("wc")
^- set right branch to newly created node
^- advance token

STEP 4. as STEP 2.

STEP 5. as STEP 3.
