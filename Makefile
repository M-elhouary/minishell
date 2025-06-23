NAME = minishell$
CC = cc
CFLAGS = -Wall -Wextra -Werror -Iinclude -Ilibft

# Source and Object Files

SRCS = src/parser/tokeniz.c\
    src/parser/utiles.c\
    src/main.c

OBJS = $(SRCS:.c=.o)

# Libft

LIBFT_DIR = libft
LIBFT_A = $(LIBFT_DIR)/libft.a

# Rules

all: $(NAME)

$(NAME): $(LIBFT_A) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT_A) -lreadline -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(LIBFT_A):
	@make -C $(LIBFT_DIR)

clean:
	rm -f $(OBJS)
	@make -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME)
	@make -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re
