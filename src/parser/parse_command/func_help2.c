/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   func_help2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houa <mel-houa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 20:02:34 by mel-houa          #+#    #+#             */
/*   Updated: 2025/08/11 12:24:55 by mel-houa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// creat new nod == new command
t_command	*create_cmd_node(char **args, t_redirection *redirections)
{
	t_command	*new_cmd;

	new_cmd = (t_command *)malloc(sizeof(t_command));
	if (!new_cmd)
		return (NULL);
	new_cmd->args = args;
	new_cmd->redirections = redirections;
	new_cmd->next = NULL;
	return (new_cmd);
}
// add new command to linkliste
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