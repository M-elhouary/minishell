/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houa <mel-houa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 16:11:58 by mel-houa          #+#    #+#             */
/*   Updated: 2025/06/27 22:10:45 by mel-houa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"

int pipe_check(t_token *tokenze)
{
        
    t_token *tmp;

    tmp = tokenze;
    if(tmp->type == PIPE && tmp)
        return ((free_token(tokenze)),
                     printf("syntax error near unexpected tokenze `|'\n"), 0);
    while(tmp && tmp->next)
    {
        if(tmp->type == PIPE && tmp->next->type == PIPE)
            return ((free_token(tokenze)),
                                printf("PIPE ERROR\n"), 0);
        tmp = tmp->next;  
    }
    if(tmp && tmp->type == PIPE)
        return ((free_token(tokenze)),
                         printf("syntax error near unexpected token `|'\n"), 0);
    return (1);
    
} 


int red_check(t_token *tokenze)
{
    t_token *tmp;
    
    tmp = tokenze;
    while(tmp)
    {
            if(((tmp->type == REDIR_IN || tmp->type == REDIR_OUT 
                || tmp->type == REDIR_APPEND || tmp->type == HEREDOC) ) 
                && (tmp->next == NULL))
                {
                    return ((free_token(tokenze)), 
                                    (printf("syntax error near unexpected token `newline'\n")), 0);
                }
            if(tmp->type == REDIR_IN && (tmp->next->type == REDIR_OUT || tmp->next->type == REDIR_IN))
                    return ((free_token(tokenze)),
                                (printf("syntax error near unexpected token `>'\n")), 0);
            if(tmp->type == REDIR_OUT
                 && (tmp->next->type == REDIR_OUT || tmp->next->type == REDIR_IN))
                    return ( (free_token(tokenze)),
                                     (printf("syntax error near unexpected token `<'\n")),0);
            tmp = tmp->next;
    }
    return (1);
}
int check_syntax_token(t_token *token)
{

    if(!pipe_check(token))
                return (0);
    if(!red_check(token))
        return (0);
    return (1);
}
