/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utiles.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houa <mel-houa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 16:04:05 by mel-houa          #+#    #+#             */
/*   Updated: 2025/06/23 16:10:15 by mel-houa         ###   ########.fr       */
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
