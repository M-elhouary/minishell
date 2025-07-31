#include "minishell.h"


t_command   *creat_cmd_nod(char **args, char **infile, char **outfile, int append, int  herdoc)
{
    t_command *new_cmd;
    new_cmd = (t_command*)malloc(sizeof(t_command));
    if(!new_cmd)
        return NULL;
    new_cmd->args = args;
    new_cmd->infile = infile;
    new_cmd->outfile = outfile;
    new_cmd->append = append;
    new_cmd->heredoc =  herdoc;
    new_cmd->next = NULL;
    return new_cmd;

}

static void add_cmd_node(t_command **head, t_command *new)
{
    t_command *tmp;
    if(!head || !new)
        return ;
    if(!*head)
        *head = new;
    else
    {
            tmp = *head;
            while (tmp->next)
                tmp = tmp->next;
            tmp->next = new;
    }

}


static void handl_red(t_token *token, t_command *cmd)
{
    int i;

    i = 0;
   
    if(!cmd)
        return ;
    while(cmd)
        cmd = cmd->next;
    if(token->type == REDIR_IN)
    {
        while(token->type == PIPE || token->type == REDIR_OUT ||  token->type == REDIR_APPEND || token->type == HEREDOC)
        {
                    cmd->infile[i] = token->next->value;
                    token = token->next->next;
                    i++;
        }
    }
    else if(token->type == REDIR_OUT)
    {
                while(token->type == PIPE || token->type == REDIR_IN ||  token->type == REDIR_APPEND || token->type == HEREDOC)
        {
                    cmd->outfile[i] = token->next->value;
                    token = token->next->next;
                    i++;
        }
    }
}

t_command	*parse_commands(t_token *tokens)
{

    t_command *cmd;
    
    
    cmd = NULL;
    char **args;
    int i;

    while (tokens)
    {
        i = 0;
        while(tokens)
        {
            if(!is_redirection(tokens->type))
            {
                args[i] = tokens->value;
                i++;
                add_cmd_node(cmd, creat_cmd_nod())
                tokens = tokens->next;
            }
            else
                break;
        }
        handl_red(tokens, cmd);
        tokens = tokens->next;
    }
    return cmd;
}