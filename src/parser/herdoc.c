/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   herdoc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houa <mel-houa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 15:18:28 by mel-houa          #+#    #+#             */
/*   Updated: 2025/08/10 16:04:21 by mel-houa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int is_delimiter_quoted(char *token_value)
{
    int len;
    
    if (!token_value)
        return (0);
    
    len = ft_strlen(token_value);
    return (token_value[0] == '\'' || token_value[0] == '\"') && 
           token_value[len-1] == token_value[0];
}

char *gen_file_name()
{
    char *name;
    char *tmp;
    
    name = ft_itoa(getpid());
    tmp = ft_strjoin("/tmp/heredoc_", name);
    free(name);
    return tmp;
}

char *similation_herdoc(char *delimiter, t_env *env_list, t_command *cmd)
{
    if (!delimiter)
        return NULL;
        
    char *line;
    int fd;
    char *file_path;
    char *expanded;
    int quoted = is_delimiter_quoted(delimiter);
    
    // If delimiter is quoted, remove the quotes
    // if (quoted)
    // {
    //     char *clean_delimiter = ft_substr(delimiter, 1, ft_strlen(delimiter) - 2);
    //     free(delimiter);
    //     delimiter = clean_delimiter;
    // }
    
    file_path = gen_file_name();
    fd = open(file_path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0)
    {
        free(file_path);
        free(delimiter);
        return NULL;
    }
    
    while (1)
    {
        line = readline("heredoc> ");
        if (!line || ft_strcmp(line, delimiter) == 0)
        {
            free(line);
            break;
        }
        
        // Only expand variables if delimiter wasn't quoted
        if (!quoted)
        {
            expanded = expand_var_in_string(line, env_list, cmd);
            free(line);
            line = expanded;
        }
        else
            expanded = line;
        
        // Write line to file with newline
        write(fd, expanded, ft_strlen(expanded));
        write(fd, "\n", 1);
        free(expanded);
    }
    
    close(fd);
    free(delimiter);
    return file_path;
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
            if (tmp->next->is_empty_expansion)
            {
                tmp = tmp->next;
                if (tmp)
                    tmp = tmp->next;
                continue;
            }
            
            pid = fork();
            if (pid < 0)
                return;
                
            if (pid == 0)
            {
                // Set signal handling for child
                signal(SIGINT, SIG_DFL);
                
                // Get heredoc content and save to file
                file_name = similation_herdoc(ft_strdup(tmp->next->value), env_list, cmd);
                if (!file_name)
                    exit(1);
                    
                exit(0);
            }
            else
            {
                int wait_result = waitpid(pid, &status, 0);
                if (wait_result == -1)
                {
                    perror("waitpid failed");
                    return;
                }
                
                // If child exited normally
                if (WIFEXITED(status) && WEXITSTATUS(status) == 0)
                {
                    // Generate file name in parent (same as child would)
                    file_name = gen_file_name();
                    
                    // Store heredoc file path
                    tmp->next->value = ft_strdup(file_name);
                    free(file_name);
                }
                else if (WIFSIGNALED(status))
                {
                    // Handle case where heredoc was interrupted
                    return;
                }
            }
        }
        
        if (tmp)
            tmp = tmp->next;
    }
}

