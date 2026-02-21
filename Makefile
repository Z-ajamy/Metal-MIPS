CC = gcc
CFLAGS = -Wall -Werror -Wextra -pedantic -std=gnu89

SRC = main.c \
	  engine/engine.c \
	  cpu/cpu.c \
	  cpu/isa/r-type/add.c \
	  cpu/isa/r-type/sub.c \
	  cpu/isa/r-type/slt.c \
	  cpu/isa/i-type/lw.c \
	  cpu/isa/i-type/sw.c \
	  cpu/isa/i-type/beq.c \
	  cpu/isa/i-type/addi.c \
	  cpu/isa/j-type/j.c \
	  io/loader.c \
	  io/display.c

OBJ = $(SRC:.c=.o)
NAME = simulator

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all
