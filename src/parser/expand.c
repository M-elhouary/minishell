/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houa <mel-houa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 22:55:46 by mel-houa          #+#    #+#             */
/*   Updated: 2025/07/11 01:13:09 by mel-houa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


char *expand_variables(const char *str, t_env *env)
{
    int i;
    int start;
    char *result;  
    char *tmp;
    char *var_name;
    char *var_value;
    char *joined;

    i = 0;
    result = ft_strdup(""); 
    while (str[i])
    {
        if (str[i] == '$')
        {
            tmp = ft_strndup(str, i);
            joined = ft_strjoin(result, tmp);
            free(result);
            free(tmp);
            result = joined;

            i++;
            start = i;
            while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
                i++;
            var_name = ft_strndup(str + start, i - start);
            var_value = get_env_value(var_name, env);
            free(var_name);
            joined = ft_strjoin(result, var_value);
            free(result);
            free(var_value);
            result = joined;
            str += i;
            i = 0;
        }
        else
        {
            tmp = ft_strndup(str, 1);
            joined = ft_strjoin(result, tmp);
            free(result);
            free(tmp);
            result = joined;
            str++;
        }
    }
    return result;
}

