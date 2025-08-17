/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houa <mel-houa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 18:50:17 by houardi           #+#    #+#             */
/*   Updated: 2025/08/12 00:15:38 by mel-houa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSE_H
# define PARSE_H

#include <sys/wait.h>
#include <errno.h>
#include <signal.h>
#include <linux/limits.h>
#include <sys/stat.h>
#include <stdio.h>
# include <unistd.h>
# include <readline/readline.h>
# include <readline/history.h>
#include <fcntl.h>
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

typedef enum e_redir_type
{
    REDIR_TYPE_IN,      // <
    REDIR_TYPE_OUT,     // >
    REDIR_TYPE_APPEND,  // >>
    REDIR_TYPE_HEREDOC  // <<
} t_redir_type;

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

typedef struct s_redirection
{
    t_redir_type type;
    char *file;
    struct s_redirection *next;
} t_redirection;

typedef struct s_command
{
    int status_exit;
    int ac;
    char *path;
    char **args;
    // Keep these for backward compatibility during transition
    // char **infile;
    // char **outfile;
    // int append;
    // int heredoc;
    t_redirection *redirections;  // New field
    struct s_command *next;
} t_command;

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

typedef struct s_token_ctx
{
    t_token **tokens;
    t_env *env;
    t_gc *gc;
    t_command *cmd;
} t_token_glbst;


// Environment functions 
char	*get_content(char *env_str);
char	*get_key(char *env_str);
t_env	*my_env(char **env);
char	*get_env_value(char *key, t_env *env);

// Token functions 
t_env	*env_new_node(char *key, char *content);
void	env_add_node(t_env **head, t_env *new);


// Token functions 
t_token	*tokenize_gc(char *line, t_env *env, t_gc *gc, t_command *cmd);
int		check_syntax_token(t_token *token, t_command *cmd);
int		free_token(t_token *token);
int		handle_quotes(const char *line, int *i);
char	*extract_word(char *line, int *i);
int	has_unclosed_quote(const char *line);
int handle_empty_expansion(t_token **tokens, t_gc *gc);
int create_and_add_token(t_token **tokens, char *value,  t_token_type type, t_gc *gc);
t_token	*create_token_gc(char *value, t_token_type type, t_gc *gc);
void	add_token(t_token **head, t_token *new);

// parse command
t_command	*parse_commands(t_token *tokens);
void		free_commands(t_command *commands);
int			check_flags(t_token *temp, int *append_flag, int *heredoc_flag);
void	count_tokens(t_token *temp, int *arg_count, int *in_count, int *out_count);
t_command	*create_cmd_node(char **args, t_redirection *redirections);
void	add_cmd_node(t_command **head, t_command *new);



// herdoc
void handl_herdoc(t_token *token, t_env *env_list, t_command *cmd);
int is_delimiter_quoted(char *token_value);
char *gen_file_name();



// Utility functions
int	check_unclosed_quote(const char *line);
char	*remove_syntactic_quotes(char *str);
char	*ft_strjoin_free(char *s1, char *s2);
int		is_metacharacter(char c);
int		is_redirection(t_token_type type);
void	print_error(char *error, char *detail);
int		is_space(char c);
char	*ft_strndup(const char *s, size_t n);
char	*expand_variables(const char *str, t_env *env, t_command *cmd);
char	**expand_and_split_gc(const char *word, t_env *env, t_gc *gc, t_command *cmd);
int		has_unquoted_variables(const char *str);
int		skip_spaces(const char *line, int *i);
int		ft_strcmp(const char *s1, const char *s2);
char	*ft_strcpy(char *dest, const char *src);
char	*remove_quotes(char *str);
char	*expand_var_in_string(const char *str, t_env *env, t_command *cmd);

// Garbage Collector functions
void	gc_init(t_gc *gc);
void	*gc_malloc(t_gc *gc, size_t size);
char	*gc_strdup(t_gc *gc, const char *s);
char	**gc_split(t_gc *gc, char const *s, char c);
void	gc_free_all(t_gc *gc);
void	gc_destroy(t_gc *gc);

char	*ft_strcat(char *dst, char *src);
int	set_env_value(t_env **env, char *key, char *value);
int	unset_env_value(t_env **env, char *key);
char	**env_to_array(t_env *env);
void	free_env_array(char **envp);

// Redirection functions
t_redirection *create_redirection(t_redir_type type, char *file);
void add_redirection(t_redirection **head, t_redirection *new_redir);
void free_redirections(t_redirection *redirections);

#endif