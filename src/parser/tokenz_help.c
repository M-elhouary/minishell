/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenz_help.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houa <mel-houa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 17:15:23 by mel-houa          #+#    #+#             */
/*   Updated: 2025/06/27 18:34:39 by mel-houa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token *handl_quote(char *line, int *i)
{
    char quote;
    int start;
    char *quote_string;
    t_token *token_quote;

    if(line[*i] != '\'' && line[*i] != '"')
        return NULL;
    quote = line[*i];
    (*i)++;
    start = *i;
    while(line[*i] && line[*i] != quote)
    {
        (*i)++;
    }
    if(!line[*i])
        return ((printf("Syntax error\n")), NULL);
    quote_string = ft_strndup(line + start, *i - start);
    (*i)++;
    token_quote = malloc(sizeof(t_token));
    if(!token_quote)
        return NULL;
    token_quote->type = WORD;
    token_quote->value = quote_string;
    token_quote->next = NULL;
    return (token_quote);
}

