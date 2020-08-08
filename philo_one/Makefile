name: philo_one

SRCS: main.c

OBJS: $(SRCS:.c=.o)

CC: gcc
CFLAGS: -Wall -Werror -Wextra

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

clean:
	rm -f $(OBJS)

fclean: clean
		rm -f $(NAME)

re: fclean all