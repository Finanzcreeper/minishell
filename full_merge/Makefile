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
	$(CC) $(CFLAGS) $(SRC) $(LIB) -o $(NAME)

testing :
	$(CC) $(SRC) $(LIB) $(CFLAGS)

clean :
	@rm minishell 2>/dev/null || true
	@rm a.out 2>/dev/null || true
	@rm infile 2>/dev/null || true
	@rm outfile 2>/dev/null || true
re : clean $(NAME)

libft :
	make -C ../libft
	make clean -C ../libft 
