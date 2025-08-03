#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "libft.h"

typedef enum e_token_type
{
	COMMAND,
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

typedef struct s_token
{
	char			*value;
	t_token_type	type;
	int				is_empty_expansion;
	struct s_token	*next;
}	t_token;

typedef struct s_command
{
	int             count;
	char 			*path;
	char			**args;
	char			**infile;
	char			**outfile;
	int				append;
	int				heredoc;
	struct s_command	*next;
}	t_command;

// Garbage Collector 
typedef struct s_gc_node
{
	void				*ptr;
	struct s_gc_node	*next;
}	t_gc_node;

typedef struct s_gc
{
	t_gc_node	*head;
}	t_gc;

// Environment functions 
char	*get_content(char *env_str);
char	*get_key(char *env_str);
t_env	*my_env(char **env);
char	*get_env_value(char *key, t_env *env);


// Token functions 
t_token	*tokenize(char *line, t_env *env);
t_token	*tokenize_gc(char *line, t_env *env, t_gc *gc);
int		check_syntax_token(t_token *token);
int		free_token(t_token *token);
int		handle_quotes(const char *line, int *i);
char	*extract_word(const char *line, int *i);

// Parsing functions
t_command	*parse_commands(t_token *tokens);
void		free_commands(t_command *commands);
 

// Utility functions
char	*remove_syntactic_quotes(char *str);
char	*ft_strjoin_free(char *s1, char *s2);
int		is_metacharacter(char c);
int		is_redirection(t_token_type type);
void	print_error(char *error, char *detail);
int		is_space(char c);
char	*ft_strndup(const char *s, size_t n);
char	*expand_variables(const char *str, t_env *env);
char	**expand_and_split(const char *word, t_env *env);
char	**expand_and_split_gc(const char *word, t_env *env, t_gc *gc);
int		has_unquoted_variables(const char *str);
int		skip_spaces(const char *line, int *i);
int		ft_strcmp(const char *s1, const char *s2);
char	*ft_strcpy(char *dest, const char *src);
char	*remove_quotes(char *str);

// Garbage Collector functions
void	gc_init(t_gc *gc);
void	*gc_malloc(t_gc *gc, size_t size);
char	*gc_strdup(t_gc *gc, const char *s);
char	**gc_split(t_gc *gc, char const *s, char c);
void	gc_free_all(t_gc *gc);
void	gc_destroy(t_gc *gc);

#endif