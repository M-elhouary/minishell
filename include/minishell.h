#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "libft.h"

typedef enum e_token_type
{
	WORD,
	PIPE,
	REDIR_IN,
	REDIR_OUT,
	REDIR_APPEND,
	HEREDOC,
	ARGUMENT
}	t_token_type;

typedef struct s_env
{
	char			*key;
	char			*content;
	struct s_env	*next;
}	t_env;
typedef struct s_exit
{
	int exit_status;
}	t_exit;

typedef struct s_flags
{
	int f_squote;
}	t_flags;

typedef struct s_token
{
	char			*value;
	t_token_type	type;
	struct s_token	*next;
}	t_token;

typedef struct s_command
{
	char			**args;
	char			*infile;
	char			*outfile;
	int				append;
	int				heredoc;
	struct s_command	*next;
}	t_command;


/* Environment functions */
t_env	*my_env(char **env);
char	*get_env_value(char *key, t_env *env);

/* Token functions */
t_token	*tokenize(char *line, t_env *env, t_flags *flags);
int		check_syntax_token(t_token *token);
int		free_token(t_token *token);
int		handle_quotes(const char *line, int *i);
char	*extract_word(const char *line, int *i, t_flags *flags);

/* Parsing functions
t_command	*parse_commands(t_token *tokens);
void		free_commands(t_command *commands);
 */

/* Utility functions */
int		is_metacharacter(char c);
int		is_redirection(t_token_type type);
void	print_error(char *error, char *detail);
int		is_space(char c);
char	*ft_strndup(const char *s, size_t n);
char	*expand_variables(const char *str, t_env *env);
int		skip_spaces(const char *line, int *i);
int		ft_strcmp(const char *s1, const char *s2);
char	*ft_strcpy(char *dest, const char *src);
char	*remove_quotes(char *str);

#endif