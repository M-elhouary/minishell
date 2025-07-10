/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houa <mel-houa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 22:55:46 by mel-houa          #+#    #+#             */
/*   Updated: 2025/07/10 19:07:56 by mel-houa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


t_env	*new_node(char *key, char *content)
{
	t_env	*new_node;

	new_node = (t_env *)malloc(sizeof(t_env));
	if (!new_node)
		return (NULL);
	new_node->key = key;
	new_node->content = content;
	new_node->next = NULL;
	return (new_node);
}


void	add_node(t_env **head, t_env *new)
{
	t_env	*tmp;

	if (!*head)
		*head = new;
	else
	{
		tmp = *head;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
	}
}


t_env *my_env(char **env)
{
    int index;
    t_env *variables;
    char *var;
    char *key;
    char *content;
    int i;
    
    index = 0;
    i = 0;
    var = NULL;
    variables = NULL;
    key = NULL;
    content = NULL;
    while(env[index])
    {
        var = ft_strdup(env[index]);
        i = 0;
        while(var[i++] != '=')
        key = ft_strndup(var, i);
        content = ft_strdup(var+(i+1));
        add_node(&(variables), new_node(key, content));
        index++;
    }
    return (variables);
}





char *expand_variables(const char *str, char **env)
{
    char *value;
    char *new_str;
    t_env *tmp;
    
    value = NULL;
    tmp = my_env(env);
    new_str = ft_strdup(str);
    
    
    if(!new_str)
        return NULL;
    while(tmp)
    {
        if(ft_strncmp(new_str, tmp->key, (ft_strlen(new_str))) == 0)//
        {
            value = tmp->content;
            break;    
        }
        tmp = tmp->next;
    }
    free(new_str);
    if(value)
        return ft_strdup(value);
    else
        return ft_strdup("");
}