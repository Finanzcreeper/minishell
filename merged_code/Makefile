CC = cc
CFLAGS = -Wall -Werror -Wextra -g3 -lreadline
MAKEFLAGS = --no-print-directory
SRC = microinterpreter.c micromain.c microparser.c proto_lexer.c proto_lexer_additional_functions.c proto_quote_handling.c
LIB = ../libft/libft.a
OBJ = $(SRC:.c=.o)
NAME = minishell

all : $(NAME)

$(NAME) :
	$(CC) $(CFLAGS) $(SRC) $(LIB)

libft :
	make -C ../libft
	make clean -C ../libft
