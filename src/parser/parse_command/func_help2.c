/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   func_help2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houa <mel-houa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 20:02:34 by mel-houa          #+#    #+#             */
/*   Updated: 2025/08/25 07:01:49 by mel-houa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_command	*create_cmd_node_gc(char **args, t_redirection *redirections,
		t_gc *gc)
{
	t_command	*new_cmd;

	new_cmd = (t_command *)gc_malloc(gc, sizeof(t_command));
	if (!new_cmd)
		return (NULL);
	new_cmd->args = args;
	new_cmd->redirections = redirections;
	new_cmd->path = NULL;
	new_cmd->status_exit = 0;
	new_cmd->print_exit = 0;
	new_cmd->next = NULL;
	return (new_cmd);
}

void	add_cmd_node(t_command **head, t_command *new)
{
	t_command	*tmp;

	if (!head || !new)
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
