NAME = minishell
CC = cc -g
CFLAGS = -Wall -Wextra -Werror -Iinclude -Ilibft

# Source and Object Files


SRCS = src/main.c \
       src/parser/tokenize.c\
       src/parser/tokenize_words.c \
       src/parser/tokenize_quotes.c \
	   src/parser/func_utils.c \
	   src/parser/word_split.c\
       src/parser/syntax_checker.c \
       src/parser/expand.c \
       src/parser/env.c\
	   src/parser/parse_command.c \
	   src/parser/tokenz_help.c \
	   src/garbage_collector.c

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
