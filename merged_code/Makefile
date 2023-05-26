CC = cc
CFLAGS = -g3 -lreadline
CFULLFLAGS =-Wall -Werror -Wextra 
MAKEFLAGS = --no-print-directory
SRC = microinterpreter.c micromain.c microparser.c proto_lexer.c proto_lexer_additional_functions.c\
proto_quote_handling.c proto_dollar_handling.c proto_dollar_handling_additional_functipons.c
LIB = ../libft/libft.a
OBJ = $(SRC:.c=.o)
NAME = minishell

all : $(NAME)

$(NAME) :
	$(CC) $(CFULLFLAGS) $(CFLAGS) $(SRC) $(LIB)

e :
	$(CC) $(CFLAGS) $(SRC) $(LIB)

libft :
	make -C ../libft
	make clean -C ../libft
