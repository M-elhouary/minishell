/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_command.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houa <mel-houa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 00:00:00 by mel-houa          #+#    #+#             */
/*   Updated: 2025/08/21 18:50:51 by mel-houa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse.h"


static void	creat_node_redir(t_redir_type type, char *value,
		t_redirection **redirections, t_gc *gc)
{
	t_redirection	*new_redir;

	new_redir = create_redirection_gc(type, value, gc);
	add_redirection(redirections, new_redir);
}

static void	help_fill_arrays(t_token *current, t_redirection **redirections, t_gc *gc)
{
	if (current->type == REDIR_IN)
	{
		current = current->next;
		if (current)
			creat_node_redir(REDIR_TYPE_IN, current->value, redirections, gc);
	}
	else if (current->type == REDIR_OUT)
	{
		current = current->next;
		if (current)
			creat_node_redir(REDIR_TYPE_OUT, current->value, redirections, gc);
	}
	else if (current->type == REDIR_APPEND)
	{
		current = current->next;
		if (current)
			creat_node_redir(REDIR_TYPE_APPEND, current->value, redirections, gc);
	}
	else if (current->type == HEREDOC)
	{
		current = current->next;
		if (current)
			creat_node_redir(REDIR_TYPE_HEREDOC, current->value, redirections, gc);
	}
}

static t_token	*fill_arrays(t_token *current, char **args,
		t_redirection **redirections, t_gc *gc)
{
	int	arg_count;

	arg_count = 0;
	while (current && current->type != PIPE)
	{
		if ((current->type == COMMAND || current->type == ARGUMENT)
			&& !current->is_empty_expansion)
			args[arg_count++] = gc_strdup(gc, current->value);
		else
		{
			help_fill_arrays(current, redirections, gc);
			current = current->next;
		}
		if (current)
			current = current->next;
	}
	args[arg_count] = NULL;
	return (current);
}

static t_token	*process_command_node(t_token *current, t_command **cmd_list, t_gc *gc)
{
	char			**args;
	t_redirection	*redirections;
	int				arg_count;

	redirections = NULL;
	arg_count = 0;
	t_token *tmp = current; 
	while (tmp && tmp->type != PIPE)
	{
		if ((tmp->type == COMMAND || tmp->type == ARGUMENT)
			&& !tmp->is_empty_expansion)
			arg_count++;
		tmp = tmp->next;
	}
	args = gc_malloc(gc, (arg_count + 1) * sizeof(char *));
	if (!args)
		return (NULL);
	current = fill_arrays(current, args, &redirections, gc);
	add_cmd_node(cmd_list, create_cmd_node_gc(args, redirections, gc));
	if (current && current->type == PIPE)
		current = current->next;
	return (current);
}

t_command	*parse_commands_gc(t_token *tokens, t_gc *gc)
{
	t_command	*cmd_list;
	t_token		*current;

	cmd_list = NULL;
	current = tokens;
	while (current)
	{
		current = process_command_node(current, &cmd_list, gc);
		if (!current && cmd_list == NULL)
			return (NULL);
	}
	return (cmd_list);
}
