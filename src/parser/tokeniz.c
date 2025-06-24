/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokeniz.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houa <mel-houa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 14:57:18 by mel-houa          #+#    #+#             */
/*   Updated: 2025/06/24 14:56:17 by mel-houa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


t_token *new_tok(char *value, t_token_type type)
{
    
    t_token *new_token;
    new_token = (t_token*)malloc(sizeof(t_token)); 
    if(!new_token)
    return NULL;
    new_token->type = type;
    new_token->value = value;
    new_token->next = NULL;
    return new_token;
    
}
void add_tok(t_token **head, t_token  *new)
{
    t_token *tmp;

    if(!*head)
         *head = new;
    else{
        tmp = *head;
        while(tmp->next)
            tmp = tmp->next;
        tmp->next = new;
    } 
}

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
        (*i)++;
    if(!line[*i])
    {
        printf("Syntax error");
        return NULL;
    }
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

t_token *tokenze(char *line)
{
    int i;
    int start;
    char *word;
    t_token *tokinzes;
    t_token *quote_token;
    
    tokinzes = NULL;
    quote_token = NULL;
    word = NULL;
    i = 0;
    while(line[i])
    {
        if(line[i] == '\'' || line[i] == '"')
        {
            quote_token = handl_quote(line, &i);
            if(!quote_token)
                return NULL;
            add_tok(&tokinzes, quote_token);
        }
        else
        {
         if(is_space(line[i]))
            i++;
        else if(line[i] == '|')
            {
                add_tok(&tokinzes,new_tok(ft_strdup("|"), PIPE));
                i++;
            }
        else if(line[i] == '<')
            {
                add_tok(&tokinzes, new_tok(ft_strdup("<"), REDIR_IN));
                i++;
            }
        else if(line[i] == '>')
            {
                 add_tok(&tokinzes, new_tok(ft_strdup(">"), REDIR_OUT));
                i++;
            }
        else if(line[i] == '>' && line[i+1] == '>')
            {
                 add_tok(&tokinzes, new_tok(ft_strdup(">>"), REDIR_APPEND));
                i += 2;
            }
        else if(line[i] == '<' && line[i+1] == '<')
            {
                 add_tok(&tokinzes, new_tok(ft_strdup("<<"), HEREDOC));
                i += 2;
            }
        else
        {
            start = i;
            while (line[i] && !is_space(line[i]) && line[i] != '>' && line[i] != '<' && line[i] != '|')
                i++;
            word = ft_strndup(line + start, i - start);
            add_tok(&tokinzes, new_tok(word, WORD));
        }
    }
}     
   return tokinzes;
}

