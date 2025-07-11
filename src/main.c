#include "minishell.h"


void print_tokens(t_token *token)
{
    const char *types[] = {
        "WORD", "PIPE", "REDIR_IN", "REDIR_OUT", "REDIR_APPEND", "HEREDOC"};

    while (token)
    {
        if (token->value)
        { 
            printf("Token: %-s\n" , token->value);
            printf("Type: %s\n", types[token->type]);
            printf("+++++++++++++++++++++++++++++++\n");
        }
        else
            printf("Token: (NULL)         | Type: %s\n", types[token->type]);
        token = token->next;
    }
}

int main(int ac, char **av, char **env)
{

    (void)ac;
    (void)av;
    char *line;
    t_token *tokens;
    t_env *env_list;

    env_list = my_env(env);

    
    
    do{
        line = readline("minishell$ ");
        add_history(line);
        if (!line)
            break;
        tokens = tokenze(line, env_list);
        print_tokens(tokens); // This might crash if token is invalid
        free(line);
    }while (1);
    return 0;

}