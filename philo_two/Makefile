NAME = philo_one

SRCS = cycle.c init.c libft.c main.c print.c utils.c

OBJS = $(SRCS:.c=.o)

CC = gcc
CFLAGS = -Wall -Werror -Wextra -pthread

.c.o:
	@$(CC) $(CFLAGS) -c -o $@ $<

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS)
	@echo "$(NAME) is compiled"

clean:
	@rm -f $(OBJS)
	@echo "object files are removed"

fclean: clean
		@rm -f $(NAME)
		@echo "$(NAME) and object files are removed"

re: fclean all

.PHONY: all fclean clean re