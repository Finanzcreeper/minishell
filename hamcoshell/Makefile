CC = cc
CFLAGS = -g3
CFULLFLAGS = -Wall -Werror -Wextra
MAKEFLAGS = --no-print-directory
SRC =	free.c\
		interpreter_3.c\
		interpreter_builtins_3.c\
		lexer_additional_functions.c\
		lexer_dollar_handling.c\
		minishell.c\
		parser_2_redirection.c\
		interpreter_1.c\
		interpreter_builtins_1.c\
		interpreter_builtins_utils_1.c\
		lexer.c\
		lexer_quote_handling.c\
		interpreter_2.c\
		interpreter_builtins_2.c\
		interpreter_builtins_utils_2.c\
		lexer_dollar_handling_additional_functions.c\
		parser_1.c
READLINE = -lreadline
LIB = ../libft/libft.a
OBJ = $(SRC:.c=.o)
NAME = minishell

all : libft $(NAME)

$(NAME) : $(OBJ)
	$(CC) $(CFULLFLAGS) $(CFLAGS) $(OBJ) $(LIB) $(READLINE) -o $(NAME)

e : libft $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(LIB) $(READLINE) -o $(NAME)

testing :
	$(CC) $(OBJ) $(LIB) $(CFLAGS) $(READLINE)

clean :
	@make clean -C ../libft 2>/dev/null
	@rm -f $(OBJ)

fclean : clean
	@rm -f minishell 2>/dev/null
	@make fclean -C ../libft 2>/dev/null

re : clean $(NAME)

libft :
	@make -C ../libft
