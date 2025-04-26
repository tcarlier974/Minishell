NAME = minishell
CC = gcc
CFLAGS = -Wall -Wextra -Werror -g
LDFLAGS = -lreadline

SRC = main.c \
		exec.c \
		./utils/utils.c \
		./utils/ft_split.c \
		./utils/f.c \
		./parsing/parse.c \
		error.c \
		./parsing/parse_lst.c \
		./parsing/init.c \


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
	make fclean
	git add .
	git commit -m "update"
	git push

.PHONY: clean fclean re