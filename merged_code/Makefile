CC = cc
CFLAGS = -g3 -L/usr/share/doc/lib32readline-dev -lreadline
CFULLFLAGS =-Wall -Werror -Wextra 
MAKEFLAGS = --no-print-directory
SRC = $(wildcard *.c)
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

clean :
	rm a.out
