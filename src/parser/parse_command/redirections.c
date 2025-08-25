/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houa <mel-houa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 16:00:00 by mel-houa          #+#    #+#             */
/*   Updated: 2025/08/25 07:00:37 by mel-houa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_redirection	*create_redirection_gc(t_redir_type type, char *file, t_gc *gc)
{
	t_redirection	*new_redir;

	new_redir = (t_redirection *)gc_malloc(gc, sizeof(t_redirection));
	if (!new_redir)
		return (NULL);
	new_redir->type = type;
	new_redir->file = gc_strdup(gc, file);
	if (!new_redir->file)
	{
		return (NULL);
	}
	new_redir->next = NULL;
	return (new_redir);
}

void	add_redirection(t_redirection **head, t_redirection *new_redir)
{
	t_redirection	*temp;

	if (!head || !new_redir)
		return ;
	if (!*head)
	{
		*head = new_redir;
		return ;
	}
	temp = *head;
	while (temp->next)
		temp = temp->next;
	temp->next = new_redir;
}

void	free_redirections(t_redirection *redirections)
{
	t_redirection	*current;
	t_redirection	*next;

	current = redirections;
	while (current)
	{
		next = current->next;
		free(current->file);
		free(current);
		current = next;
	}
}
