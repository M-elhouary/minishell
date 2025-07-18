/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utiles.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houa <mel-houa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 16:04:05 by mel-houa          #+#    #+#             */
/*   Updated: 2025/07/11 00:42:57 by mel-houa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include "minishell.h"

char *ft_strndup(const char *s, size_t n)
{
    size_t i;
    char *dup;

    if (!s)
        return NULL;
    dup = malloc(n + 1);
    if (!dup)
        return NULL;
    i = 0;
    while (i < n && s[i])
    {
        dup[i] = s[i];
        i++;
    }
    dup[i] = '\0';
    return dup;
}   
int is_space(char c)
{
    return ((c >= 9 && c <= 13 ) || c == 32);
}


int free_token(t_token *token)
{
    t_token *tmp;

    while(token)
    {
        tmp = token;
        token = token->next;
        if(tmp->value)
            free(tmp->value);
        free(tmp);
    }
    return (1);
}


int ft_strcmp(const char *s1, const char *s2) 
{
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return (unsigned char)(*s1) - (unsigned char)(*s2);
}


char *ft_strcpy(char *dest, const char *src)
 {
    char *start = dest;

    while (*src) {
        *dest++ = *src++;
    }
    *dest = '\0';  // Add null terminator
    return start;
}
