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
SRCS_PARSE = src/main.c \
       src/parser/tokens/tokenize.c\
       src/parser/tokens/tokenize_words.c \
       src/parser/tokens/tokenize_quotes.c \
	   src/parser/tokens/tokenz_help.c \
	   src/parser/tokens/tokenz_help2.c \
       src/parser/expansion/expand.c \
	   src/parser/expansion/word_split.c\
	   src/parser/expansion/utils.c \
       src/parser/environment/env.c\
       src/parser/environment/func_help.c\
	   src/parser/utils/func_utils.c \
	   src/parser/utils/func_utils2.c \
	   src/parser/syntax_checker.c\
	   src/parser/parse_command/parse_command.c \
	   src/parser/parse_command/func_help.c\
	   src/parser/parse_command/func_help2.c\
	   src/parser/parse_command/redirections.c\
	   src/parser/herdoc/herdoc.c\
	   src/parser/herdoc/utils.c\
	   src/garbage_collector.c\
	   src/parser/signals.c \


SRCS_EXEC = src/exec/utils.c \
			src/exec/locate_cmd.c \
			src/exec/exec_cmd.c \
			src/exec/builtin_functs.c \
			src/exec/builin.c \
			src/exec/cd.c \
			src/exec/export.c \
			src/exec/_env.c \
			src/exec/print.c \
			src/exec/pipe.c \
			src/exec/redirections.c

SRCS = $(SRCS_PARSE) $(SRCS_EXEC)
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
	@find . -name '*.o' -delete
	@make -C $(LIBFT_DIR) clean -s
	@printf "\033[0;32m✓ Clean completed\033[0m\n"

fclean: clean
	@printf "\033[0;31mRemoving $(NAME)...\033[0m\n"
	@rm -f $(NAME) parse
	@make -C $(LIBFT_DIR) fclean -s
	@printf "\033[0;32m✓ Full clean completed\033[0m\n"

re: fclean all

.PHONY: all clean fclean re
.PHONY: all clean fclean re
