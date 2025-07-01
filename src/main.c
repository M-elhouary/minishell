#include "minishell.h"


void print_tokens(t_token *token)
{
    const char *types[] = {
        "WORD", "PIPE", "REDIR_IN", "REDIR_OUT", "REDIR_APPEND", "HEREDOC"};

    while (token)
    {
        if (token->value)
        {

        
            printf("Token: %-15s\n" , token->value);
            printf("Type: %s\n", types[token->type]);
            printf("+++++++++++++++++++++++++++++++\n");
        }
        else
            printf("Token: (NULL)         | Type: %s\n", types[token->type]);
        token = token->next;
    }
}

int main(void)
{
    char *line;
    t_token *tokens;

    
    
    do{
        line = readline("minishell$ ");
        add_history(line);
        if (!line)
            break;
        tokens = tokenze(line);
        print_tokens(tokens); // This might crash if token is invalid
        free(line);
    }while (1);
    return 0;

//    char *line = "'unclosed";
//     int i = 0;

//     t_token *tok = handl_quote(line, &i);

//     if (tok)
//     {
//         printf("Token: [%s]\n", tok->value);
//         printf("Index after quote: %d\n", i);
//         free(tok->value);
//         free(tok);
//     }
//     else
//     {
//         printf("Quote parsing failed.\n");
//     }

//     return 0;
}

