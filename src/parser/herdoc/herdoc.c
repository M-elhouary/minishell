/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   herdoc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houa <mel-houa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 15:18:28 by mel-houa          #+#    #+#             */
/*   Updated: 2025/08/13 18:12:43 by mel-houa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// signal

void similation_herdoc(char *delimiter, int fd, t_env *env_list, t_command *cmd)
{
    char *expanded;
    char *line;
    char *clean_delimiter;
    int quotes_for_expansion = 0;  // Flag to check if we should expand variables
    
    // 1. Make a copy of the original delimiter
    clean_delimiter = ft_strdup(delimiter);
    if (!clean_delimiter)
    {
        close(fd);
        free(delimiter);
        return;
    }
    
    // 2. Check if delimiter has quotes (for variable expansion control)
    quotes_for_expansion = has_quotes(delimiter);
    
    // 3. Always remove all quotes for comparison purposes
    char *no_quotes = remove_quotes(clean_delimiter);
    //free(clean_delimiter);
    clean_delimiter = no_quotes;
    
    while (1)
    {
        line = readline("herdoc>");
        if (!line)
            break;
        
        // Compare user input with delimiter (without quotes)
        if (ft_strcmp(line, clean_delimiter) == 0)
        {
            free(line);
            break;
        }
        // Only expand variables if original delimiter had no quotes
        if (!quotes_for_expansion)
        {
            expanded = expand_var_in_string(line, env_list, cmd);
            free(line);
        }
        else
            expanded = line;
            
        write(fd, expanded, ft_strlen(expanded));
        write(fd, "\n", 1);
        free(expanded);
    }
    
    free(clean_delimiter);
    close(fd);
    free(delimiter);
}


void handl_herdoc(t_token *token, t_env *env_list, t_command *cmd)
{
    t_token *tmp;
    pid_t pid;
    int wait_result;
    int status;
    char *file_name;
    int fd;

    
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
            file_name = gen_file_name();
            fd = open(file_name, O_WRONLY | O_CREAT, 0600);
            if (fd < 0)
            {
                free(file_name);
                return ;
            }
            pid = fork();
            if (pid < 0)
                return;
     
            if (pid == 0)
            {
                similation_herdoc(ft_strdup(tmp->next->value), fd, env_list, cmd);
                exit(0);
            }
            wait_result = waitpid(pid, &status, 0);
            if (wait_result == -1)
            {
                 perror("waitpid failed");
                return;
            }
            if (WIFEXITED(status) && WEXITSTATUS(status) == 0)
            {
                tmp->next->value = ft_strdup(file_name);
                free(file_name);
            }
            else if (WIFSIGNALED(status))
                return ;
        }
        if (tmp)
            tmp = tmp->next;
    }
}
