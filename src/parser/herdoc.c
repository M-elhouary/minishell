/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   herdoc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houa <mel-houa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 15:18:28 by mel-houa          #+#    #+#             */
/*   Updated: 2025/08/09 18:58:36 by mel-houa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


char *gen_file_name()
{
    char *name;
    name = ft_itoa(getpid() + 12344);
    name = ft_strjoin("/tmp/herdoc.c", name);
    return name;
}

int  similation_herdoc(char *delimeter)
{

    if(!delimeter)
        return 1;
    size_t len_d ;
    char *line;
    char *tmp;
    int fd;
    char *file_name;

    
    len_d =  ft_strlen(delimeter);
    file_name = gen_file_name();
    fd =  open("file_name", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if(fd < 0)
        return 1;
    while (1)
    {
        line = readline("herdoc>");
        tmp = line;
        if(ft_strncmp(tmp, delimeter, len_d) == 0)
           return 1;
        write(fd, tmp, ft_strlen(line));
        free(line);
    }

    // expand only unquoted del
    // remove node herdoc replace by file holde content 
}


void handl_herdoc(t_token *token)
{
    t_token *tmp;
    pid_t pid;
    int status;

    tmp = token;
    if (!tmp)
        return;
    while (tmp)
    {
        if (tmp->type == HEREDOC)
        {
            pid = fork();
            if (pid < 0)
            {
                perror("fork failed");
                return;
            }
            if (pid == 0)
            {
                if(similation_herdoc(tmp->next->value))
                    exit(0);
            }
            else
            {
                if (waitpid(pid, &status, 0) == -1)
                    perror("waitpid failed");

            }
        }
        tmp = tmp->next;
    }
}

