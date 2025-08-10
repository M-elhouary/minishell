/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   herdoc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houa <mel-houa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 15:18:28 by mel-houa          #+#    #+#             */
/*   Updated: 2025/08/10 15:25:12 by mel-houa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


// loop on token 
// if type == HEREDOC
// 1. fork
// generate file name
// 2. read from stdin until dilimiter
// 3. write to file
// 4. close file
// exit child
// 5. replace HEREDOC token with file name



int is_delimiter_quoted(char *token_value)
{
    int len;
    len = ft_strlen(token_value);
    return (token_value[0] == '\'' || token_value[0] == '\"') && 
           token_value[len-1] == token_value[0];
}

char *gen_file_name()
{
    char *name;
    name = ft_itoa(getpid() + 12344);
    return ft_strjoin("/tmp/herdoc_", name);
}

char   *similation_herdoc(char *dilimiter, t_env *env_list, t_command *cmd)
{

    if(!dilimiter)
        return NULL;
    char *line;
    int fd;
    char *file_path;
    char *expanded;
    
    file_path = gen_file_name();
    fd =  open(file_path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if(fd < 0)
        return NULL;
    while (1)
    {
        line = readline("herdoc>");
        if(!line || ft_strcmp(line, dilimiter) != 0)
        {
            free(line);
            break;
        }
        if (!is_delimiter_quoted(line))
        {
            expanded = expand_var_in_string(line, env_list, cmd);
            write(fd, expanded, ft_strlen(expanded));
            free(expanded);
        }
        else
        {
            write(fd, line, ft_strlen(line));
            write(fd, "\n", 1);
            free(line);
        }
    }
    close(fd);
    return (exit(0), file_path);
    // expand only unquoted del
    // remove node herdoc replace by file holde content 
}


void handl_herdoc(t_token *token, t_env *env_list, t_command *cmd)
{
    t_token *tmp;
    pid_t pid;
    int status;
    char *file_name;

    tmp = token;
    if (!tmp)
        return;
    while (tmp)
    {
        if (tmp->type == HEREDOC && tmp->next && tmp->next->type == ARGUMENT)
        {
            // If HEREDOC token is followed by an argument, we handle it
            if (tmp->next->is_empty_expansion)
            {
                // If the next token is empty expansion, we skip it
                tmp = tmp->next;
                continue;
            }
    
            pid = fork();
            if (pid < 0)
                return;
            if (pid == 0)
            {
                file_name = similation_herdoc(tmp->next->value, env_list, cmd);
                if (!file_name)
                    continue;
                else
                {
                    free(tmp->value);
                    tmp->value = file_name; 
                    tmp->type = ARGUMENT;
                    tmp = tmp->next;
                 }
            }
            else
            {
                if (waitpid(pid, &status, 0) == -1)
                    perror("waitpid failed");

            }
            
            // replace HEREDOC token with file name
        }
        tmp = tmp->next;
    }
}

