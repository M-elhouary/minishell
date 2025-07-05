#ifndef MINISHELL_H
#define MINISHELL_H



#include <stdio.h>
#include <stdlib.h>
#include <libft.h>
#include <readline/readline.h>
#include <readline/history.h>


typedef enum e_token_type
{
    WORD,
    PIPE,
    REDIR_IN,      // <
    REDIR_OUT,     // >
    REDIR_APPEND,  // >>
    HEREDOC        // <<
}   t_token_type;

typedef struct s_env
{
    char *key;
    char *content;
    struct s_env *next; 
} t_env;

typedef struct s_token
{
    char            *value;
    t_token_type    type;
    struct s_token  *next;
}   t_token;

typedef struct s_command
{
    char            **args;
    char            *infile;
    char            *outfile;
    int             append;   // 1 if >>
    int             heredoc;  // 1 if <<
    struct s_command *next;
}   t_command;



char *ft_strndup(const char *s, size_t n);
t_env *my_env(char **env);
t_token *handl_quote(char *line, int *i);
int check_syntax_token(t_token *token);
int free_token(t_token *token);
char *extract_word(const char *line, int *i);
char *expand_variables(const char *str, char **env);
int is_space(char c);
t_token *tokenze(char *line, char **env);
#endif
