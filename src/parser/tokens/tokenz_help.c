/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenz_help.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houa <mel-houa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 17:15:23 by mel-houa          #+#    #+#             */
/*   Updated: 2025/08/12 00:14:41 by mel-houa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"



char	*remove_syntactic_quotes(char *str)
{
	int		len;
	char	*new;
	
	if (!str)
		return (NULL);
	
	len = ft_strlen(str);
	if (len >= 2)
	{
		// Only remove matching outer quotes
		if ((str[0] == '\'' && str[len-1] == '\'') ||
			(str[0] == '"' && str[len-1] == '"'))
		{
			new = ft_strndup(str+1, len-2);
			free(str);
			return (new);
		}
	}
	return (str);
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

// Create empty expansion token for variable that expands to nothing
int handle_empty_expansion(t_token **tokens, t_gc *gc)
{
    t_token *new;

    new = create_token_gc(gc_strdup(gc, ""), ARGUMENT, gc);
    if (!new)
        return (0);
    new->is_empty_expansion = 1;
    add_token(tokens, new);
    return (1);
}

// Helper function to create and add a token in one step
int create_and_add_token(t_token **tokens, char *value,  t_token_type type, t_gc *gc)
{
    t_token *new;
    char *dup_value;

    dup_value = gc_strdup(gc, value);
    if (!dup_value)
        return (0);
        
    new = create_token_gc(dup_value, type, gc);
    if (!new)
        return (0);
        
    add_token(tokens, new);
    return (1);
}
