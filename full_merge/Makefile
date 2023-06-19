CC = cc
CFLAGS = -g3 -lreadline
CFULLFLAGS = -Wall -Werror -Wextra
MAKEFLAGS = --no-print-directory
SRC = $(wildcard *.c)
LIB = ../libft/libft.a
OBJ = $(SRC:.c=.o)
NAME = minishell

all : $(NAME)

$(NAME) :
	$(CC) $(CFULLFLAGS) $(CFLAGS) $(SRC) $(LIB) -o $(NAME)

e :
	$(CC) $(CFLAGS) $(SRC) $(LIB)

testing :
	$(CC) $(CFLAGS) $(SRC) $(LIB)

clean :
	- rm minishell
	- rm a.out

re : clean $(NAME)

libft :
	make -C ../libft
	make clean -C ../libft
