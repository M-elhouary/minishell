/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   herdoc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houa <mel-houa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 15:18:28 by mel-houa          #+#    #+#             */
/*   Updated: 2025/08/27 22:31:38 by mel-houa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	process_heredoc_token(t_token *tmp, t_env *env_list, t_command *cmd,
		t_gc *gc)
{
	char				*file_name;
	t_heredoc_params	params;
	int					fd;

	params = (t_heredoc_params){env_list, cmd, gc};
	if (fork_for_file_name(tmp, gc, &file_name, &fd) == -1)
		return (0);
	cmd->status_exit = execute_heredoc_child(tmp, fd, &params);
	tmp->next->value = gc_strdup(gc, file_name);
	return (cmd->status_exit);
}

int	handl_herdoc_gc(t_token *token, t_env *env_list, t_command *cmd, t_gc *gc)
{
	t_token	*tmp;

	tmp = token;
	if (!tmp)
		return (1);
	while (tmp)
	{
		if (tmp->type == HEREDOC && tmp->next && tmp->next->type == ARGUMENT
			&& !tmp->next->is_empty_expansion)
		{
			if (process_heredoc_token(tmp, env_list, cmd, gc))
				return (1);
		}
		if (tmp)
			tmp = tmp->next;
	}
	return (0);
}
