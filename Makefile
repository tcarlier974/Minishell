NAME = minishell
CC = gcc
CFLAGS = -Wall -Wextra -Werror
LDFLAGS = -lreadline

SRC = main.c \
		exec.c \
		./utils/utils.c \
		./utils/ft_split.c \
		parse.c \

OBJ = $(SRC:.c=.o)

$(NAME): $(OBJ)
	$(CC) $(OBJ) $(LDFLAGS) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean $(NAME)

git:
	git add .
	git commit -m "update"
	git push

.PHONY: clean fclean re