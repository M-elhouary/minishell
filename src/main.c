#include "minishell.h"


void print_tokens(t_token *token)
{
    while (token)
    {
        printf("Token: %-10s Type: %d\n", token->value, token->type);
        token = token->next;
    }
}


int main(void)
{
    char *line;
    t_token *tokens;

    while (1)
    {
        line = readline("minishell$ ");
        if (!line)
            break;
        tokens = tokenze(line);
        print_tokens(tokens);
    }
    return 0;
}