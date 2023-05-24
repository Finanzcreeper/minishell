CC = cc
CFLAGS = -Wall -Werror -Wextra -g3
MAKEFLAGS = --no-print-directory
SRC = proto_lexer.c proto_lexer_additional_functions.c
LIB = ../libft/libft.a
OBJ = $(SRC:.c=.o)
NAME = minishell

all : $(NAME)

$(NAME) :
	$(CC) $(CFLAGS) $(SRC) $(LIB)

libft :
	make -C ../libft
	make clean -C ../libft
