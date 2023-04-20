# Microshell

## LEXER
+ v1: only two separators ' ' '|'
+ v1: only two types i.e. T_WORD, T_PIPE
+ v2: only four separators ' ' '|' '<' and '>'
+ v2: only four types i.e. T_WORD, T_PIPE, T_REDIR_FROM, T_REDIR_TO
- no exit codes
- no quote handling
- no $ arg expansions

## PARSER
- creates an AST from a super-reduced grammar in Backus Naur form i.e.

pipe_sequence	: cmd_word 
				| pipe_sequence PIPE cmd_word
				;
cmd_word		: WORD
				;

- output AST should have only two node types, PIPE and CMD
- CMD's will have their own struct consisting of v1: command, args + v2: infile and outfile (std if not redirected)

## INTERPRETER
- traverses the AST from the root node
	- executes CMD nodes
	- forks subshells when PIPE nodes encountered
	- 
- differentiates between simple and compound commands?

# Module Specification for twagger minishell

## LEXER
- takes in an input string and returns a linked list of tokens
- where each token has:
	- content (i.e."echo", ">")
	- a type (T_WORD, T_RED_TO)

## PARSER
- takes in the tokens and produces an Abstract Syntax Tree

- define a set of grammar rules, i.e. reduced version of Backus Naur form, as defined here:
- https://pubs.opengroup.org/onlinepubs/9699919799.2018edition/utilities/V3_chap02.html#tag_18_10
- https://en.wikipedia.org/wiki/Backus%E2%80%93Naur_form
use bison to generate a textual automaton
manually generate a parsing table from the textual automaton

ms_parser
- takes in a linked list of tokens and the parsing table
- uses Shift Reduce Parsing to produce an Abstract Syntax Tree used by the interpreter
- https://www.tutorialspoint.com/what-is-shift-reduce-parser

# INTERPRETER
- takes in an AST and traverses it in post-order to execute the shell commands in the right order
- 