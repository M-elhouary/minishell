NAME = minishell
CC = cc -g
CFLAGS = -Wall -Wextra -Werror -Iinclude -Ilibft

# Colors for output
GREEN = \033[0;32m
BLUE = \033[0;34m
YELLOW = \033[0;33m
RED = \033[0;31m
NC = \033[0m

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
	@printf "\033[0;34mLinking $(NAME)...\033[0m\n"
	@$(CC) $(CFLAGS) $(OBJS) $(LIBFT_A) -lreadline -o $(NAME)
	@printf "\033[0;32m✓ $(NAME) compiled successfully!\033[0m\n"

%.o: %.c
	@printf "\033[0;33mCompiling $<\033[0m\n"
	@$(CC) $(CFLAGS) -c $< -o $@

$(LIBFT_A):
	@printf "\033[0;34mBuilding libft...\033[0m\n"
	@make -C $(LIBFT_DIR) -s

clean:
	@printf "\033[0;31mCleaning object files...\033[0m\n"
	@rm -f $(OBJS)
	@make -C $(LIBFT_DIR) clean -s
	@printf "\033[0;32m✓ Clean completed\033[0m\n"

fclean: clean
	@printf "\033[0;31mRemoving $(NAME)...\033[0m\n"
	@rm -f $(NAME)
	@make -C $(LIBFT_DIR) fclean -s
	@printf "\033[0;32m✓ Full clean completed\033[0m\n"

re: fclean all

.PHONY: all clean fclean re
