/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houa <mel-houa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 12:03:01 by mel-houa          #+#    #+#             */
/*   Updated: 2025/08/11 12:03:26 by mel-houa         ###   ########.fr       */
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