/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houa <mel-houa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 00:00:00 by mel-houa          #+#    #+#             */
/*   Updated: 2025/08/27 22:28:38 by mel-houa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSE_H
# define PARSE_H

# include "libft.h"
# include <errno.h>
# include <fcntl.h>
# include <linux/limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <unistd.h>

// =================================================== Forward Declarations ===

typedef struct s_env			t_env;
typedef struct s_gc				t_gc;
typedef struct s_command		t_command;
typedef struct s_redirection	t_redirection;
typedef struct s_token_ctx		t_token_ctx;

// =================================================Tokens struct && enum =====

typedef enum e_token_type
{
	COMMAND,
	PIPE,
	REDIR_IN,
	REDIR_OUT,
	REDIR_APPEND,
	HEREDOC,
	ARGUMENT
}								t_token_type;

typedef struct s_token
{
	char						*value;
	t_token_type				type;
	int							is_empty_expansion;
	int							prev_herdoc;
	struct s_token				*next;
}								t_token;

typedef struct s_token_ctx
{
	t_token						**tokens;
	t_env						*env;
	t_gc						*gc;
	t_command					*cmd;
}								t_token_glbst;

// ================================================== parse command struct ====

typedef struct s_command
{
	int							status_exit;
	int							ac;
	char						*path;
	char						**args;
	int							print_exit;
	t_redirection				*redirections;
	struct s_command			*next;
}								t_command;

typedef enum e_redir_type
{
	REDIR_TYPE_IN,
	REDIR_TYPE_OUT,
	REDIR_TYPE_APPEND,
	REDIR_TYPE_HEREDOC
}								t_redir_type;

typedef struct s_redirection
{
	t_redir_type				type;
	char						*file;
	struct s_redirection		*next;
}								t_redirection;

// ================================================ Garbage Collector struct ==

typedef struct s_gc_node
{
	void						*ptr;
	struct s_gc_node			*next;
}								t_gc_node;

typedef struct s_gc
{
	t_gc_node					*head;
}								t_gc;

//  =============================================== herdoc  struct ============

typedef struct s_heredoc_ctx
{
	char						*delimiter;
	int							fd;
	t_env						*env_list;
	t_command					*cmd;
	t_gc						*gc;
}								t_heredoc_ctx;

typedef struct s_heredoc_params
{
	t_env						*env_list;
	t_command					*cmd;
	t_gc						*gc;
}								t_heredoc_params;

typedef struct s_var_ctx
{
	t_env						*env;
	t_gc						*gc;
}								t_var_ctx;

typedef struct s_clean
{
	t_env						*env;
	t_command					*cmd;
	t_gc						*gc;
	char						*path;
	int							*pid;
}								t_clean;

// ================================================ Garbage Collector functions

void							gc_init(t_gc *gc);
void							*gc_malloc(t_gc *gc, size_t size);
char							*gc_strdup(t_gc *gc, const char *s);
char							*gc_strndup(t_gc *gc, const char *s, size_t n);
char							**gc_split(t_gc *gc, char const *s, char c);
void							gc_free_all(t_gc *gc);
void							gc_destroy(t_gc *gc);
char							*ft_strcat(char *dst, char *src);

// =================================================== Environment ============

typedef struct s_env
{
	char						*key;
	char						*content;
	int							exported_only;
	struct s_env				*next;
}								t_env;

char							*get_content(char *env_str);
char							*get_key(char *env_str);
t_env							*my_env(char **env);
char							*get_env_value(char *key, t_env *env);
int								set_env_value(t_env **env, char *key,
									char *value);
int								unset_env_value(t_env **env, char *key);
char							**env_to_array(t_env *env);
void							free_env_array(char **envp);
void							cleanup_env(t_env *env_list);

// ================================================= Tokens functions =========

t_env							*env_new_node(char *key, char *content);
void							env_add_node(t_env **head, t_env *new);
t_token							*tokenize_gc(char *line, t_env *env, t_gc *gc,
									t_command *cmd);
int								check_syntax_token(t_token *token,
									t_command *cmd);
int								free_token(t_token *token);
int								handle_quotes(const char *line, int *i);
char							*extract_word_gc(char *line, int *i, t_gc *gc);
int								has_unclosed_quote(const char *line);
int								handle_empty_expansion(t_token **tokens,
									t_gc *gc);
int								create_and_add_token(t_token **tokens,
									char *value, t_token_type type, t_gc *gc);
t_token							*create_token_gc(char *value, t_token_type type,
									t_gc *gc);
void							add_token(t_token **head, t_token *new);
t_token							*get_last_token(t_token_ctx *ctx);
int								add_split_words(t_token **tokens,
									char **split_words, t_gc *gc);
int								handle_double_char_op(t_token **tokens,
									char *line, int *i, t_gc *gc);

// ================================================== parse command function ==

t_command						*parse_commands_gc(t_token *tokens, t_gc *gc);
void							free_commands(t_command *commands);
int								check_flags(t_token *temp, int *append_flag,
									int *heredoc_flag);
void							count_tokens(t_token *temp, int *arg_count,
									int *in_count, int *out_count);
t_command						*create_cmd_node_gc(char **args,
									t_redirection *redirections, t_gc *gc);
void							add_cmd_node(t_command **head, t_command *new);

// =================================================== Herdoc functions =======

typedef struct s_expand_ctx
{
	t_env						*env;
	t_command					*cmd;
	t_gc						*gc;
}								t_expand_ctx;

int								get_herdoc_fd(int fd);
int								execute_heredoc_child(t_token *tmp, int fd,
									t_heredoc_params *params);
void							similation_herdoc(char *delimiter, int fd,
									t_heredoc_params *params);
void							heredoc_readline_loop(t_heredoc_ctx *ctx,
									char *clean_delimiter,
									int quotes_for_expansion);
int								fork_for_file_name(t_token *tmp, t_gc *gc,
									char **file_name, int *fd);
void							process_herdoc_line(char *line, int fd,
									t_heredoc_ctx *ctx);
void							write_raw_line(char *line, int fd);
int								process_readline_input(char *line,
									char *clean_delimiter);
int								handl_herdoc_gc(t_token *token, t_env *env_list,
									t_command *cmd, t_gc *gc);
int								is_delimiter_quoted(char *token_value);
char							*gen_file_name_gc(char *s, int random_nb,
									t_gc *gc);
void							free_and_close_gc(char *clean_delimiter, int fd,
									char *delimiter, t_gc *gc);
int								prepare_delimiter_gc(char **clean_delimiter,
									char *delimiter, int *quotes_flag,
									t_gc *gc);

//  ===============================================expansion ==================

char							*handle_exit_status_gc(char **result, int *i,
									t_command *cmd, t_gc *gc);
char							*expand_variables_gc(const char *str,
									t_env *env, t_command *cmd, t_gc *gc);
char							**expand_and_split_gc(const char *word,
									t_env *env, t_gc *gc, t_command *cmd);
char							*expand_var_in_string_gc(const char *str,
									t_env *env, t_command *cmd, t_gc *gc);
int								has_unquoted_variables(const char *str);
char							*handle_variable_gc(char **result,
									const char *str, int *i, t_var_ctx *ctx);

//  =============================================== Utility functions =========

int								has_quotes(const char *str);
char							*remove_syntactic_quotes(char *str);
char							*ft_strjoin_free(char *s1, char *s2);
char							*ft_strjoin_free_gc(char *s1, char *s2,
									t_gc *gc);
int								is_metacharacter(char c);
int								is_operater(t_token_type type);
void							print_error(char *error, char *detail);
int								is_space(char c);
char							*ft_strndup(const char *s, size_t n);
char							*ft_strndup_gc(t_gc *gc, const char *s,
									size_t n);
int								skip_spaces(const char *line, int *i);
int								ft_strcmp(const char *s1, const char *s2);
char							*ft_strcpy(char *dest, const char *src);
char							*remove_quotes(char *str);
char							*remove_quotes_gc(char *str, t_gc *gc);

//  =================================================Redirection functions ====

t_redirection					*create_redirection_gc(t_redir_type type,
									char *file, t_gc *gc);
void							add_redirection(t_redirection **head,
									t_redirection *new_redir);
void							free_redirections(t_redirection *redirections);

//  ======================================================== signals ==========

void							sigint_interactive(int sig);

#endif