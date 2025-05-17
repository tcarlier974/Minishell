NAME = minishell

CC = cc
CFLAGS = -Wall -Wextra -Werror -I./includes -L ./libft -g3
LDFLAGS = -lreadline -lft

SRC = ./builtin/builtins1.c \
	./builtin/builtins2.c \
	./builtin/builtins3.c \
	./env/env1.c \
	./env/env2.c \
	./env/file_executor.c \
	./execute/execute1.c \
	./execute/execute2.c \
	./execute/execute3.c \
	./execute/execute4.c \
	./main/main1.c \
	./main/main2.c \
	./main/main3.c \
	./main/main4.c \
	./main/main5.c \
	./main/main6.c \
	./main/main7.c \
	./parse/heredoc.c \
	./parse/parse1.c \
	./parse/parse2.c \
	./parse/parse3.c \
	./parse/parse4.c \
	./parse/parse5.c \
	./parse/parse6.c \
	./utils/utils1.c \
	./utils/utils2.c \
	./utils/utils3.c \
	./utils/utils4.c \
	./utils/utils5.c

OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	make libft
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

libft:
	@make -C libft

re: fclean all

.PHONY: all clean fclean re libft
