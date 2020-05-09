NAME = ssu_mntr
SRCS = main.c ssu_mntr.c
OBJS = $(SRCS:.c=.o)

CC = gcc

.PHONY = all clean fclean re

all: $(NAME)

$(NAME) : $(OBJS)

.c.o: ssu_mntr.h
	$(CC) -c -g $< -I.

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(NAME)

re: fclean all
