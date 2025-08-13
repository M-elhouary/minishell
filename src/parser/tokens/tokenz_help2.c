/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenz_help2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houa <mel-houa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 00:13:38 by mel-houa          #+#    #+#             */
/*   Updated: 2025/08/12 21:09:09 by mel-houa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Create a new token, allocating memory using the garbage collector
t_token	*create_token_gc(char *value, t_token_type type, t_gc *gc)
{
	t_token	*new;

	new = gc_malloc(gc, sizeof(t_token));
	if (!new)
		return (NULL);
	new->value = value;
	new->type = type;
	new->is_empty_expansion = 0;
	new->next = NULL;
	return (new);
}


// Add a token to the end of the token linked list
void	add_token(t_token **head, t_token *new)
{
	t_token	*tmp;

	if (!new)
		return ;
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