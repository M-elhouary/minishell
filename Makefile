NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror -Iinclude -Ilibft

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
	   src/parser/utils/func_utils3.c \
	   src/parser/syntax_checker.c\
	   src/parser/parse_command/parse_command.c \
	   src/parser/parse_command/func_help.c\
	   src/parser/parse_command/func_help2.c\
	   src/parser/parse_command/redirections.c\
	   src/parser/herdoc/herdoc.c\
	   src/parser/herdoc/utils.c\
	   src/parser/herdoc/utils2.c\
	   src/parser/herdoc/utils3.c\
	   src/garbage_c/garbage_collector.c\
	   src/garbage_c/gc_help_func.c\
	   src/garbage_c/gc_help_func2.c\
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

LIBFT_DIR = libft
LIBFT_A = $(LIBFT_DIR)/libft.a

all: $(NAME)

$(NAME): $(LIBFT_A) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT_A) -lreadline -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(LIBFT_A):
	make -C $(LIBFT_DIR)

clean:
	rm -f $(OBJS)
	make -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME) parse
	make -C $(LIBFT_DIR) fclean
re: fclean all

.PHONY: all clean fclean re