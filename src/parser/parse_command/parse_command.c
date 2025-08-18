/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_command.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houa <mel-houa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 00:00:00 by mel-houa          #+#    #+#             */
/*   Updated: 2025/08/14 00:34:18 by mel-houa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse.h"

// static int	allocate_arrays(char ***args, char ***infiles, char ***outfiles,
//		int arg_count, int in_count, int out_count)
// {
// 	*args = malloc((arg_count + 1) * sizeof(char *));
// 	*infiles = malloc((in_count + 1) * sizeof(char *));
// 	*outfiles = malloc((out_count + 1) * sizeof(char *));
// 	if (!*args || !*infiles || !*outfiles)
// 		return (0);
// 	return (1);
// }

// static void init_index_array(int	*arg_count, int	*in_count, int	*out_count)
// {

// 	*arg_count = 0;
// 	*in_count = 0;
// 	*out_count = 0;
// }

static void	creat_node_redir(t_redir_type type, char *value,
		t_redirection **redirections)
{
	t_redirection	*new_redir;

	new_redir = create_redirection(type, value);
	add_redirection(redirections, new_redir);
}

static void	help_fill_arrays(t_token *current, t_redirection **redirections)
{
	if (current->type == REDIR_IN)
	{
		current = current->next;
		if (current)
			creat_node_redir(REDIR_TYPE_IN, current->value, redirections);
	}
	else if (current->type == REDIR_OUT)
	{
		current = current->next;
		if (current)
			creat_node_redir(REDIR_TYPE_OUT, current->value, redirections);
	}
	else if (current->type == REDIR_APPEND)
	{
		current = current->next;
		if (current)
			creat_node_redir(REDIR_TYPE_APPEND, current->value, redirections);
	}
	else if (current->type == HEREDOC)
	{
		current = current->next;
		if (current)
			creat_node_redir(REDIR_TYPE_HEREDOC, current->value, redirections);
	}
}

static t_token	*fill_arrays(t_token *current, char **args,
		t_redirection **redirections)
{
	int	arg_count;

	arg_count = 0;
	while (current && current->type != PIPE)
	{
		if ((current->type == COMMAND || current->type == ARGUMENT)
			&& !current->is_empty_expansion)
			args[arg_count++] = ft_strdup(current->value);
		else
		{
			help_fill_arrays(current, redirections);
			current = current->next;
		}
		if (current)
			current = current->next;
	}
	args[arg_count] = NULL;
	return (current);
}

// Helper to process a single command in the parse_commands loop
static t_token	*process_command_node(t_token *current, t_command **cmd_list)
{
	char			**args;
	t_redirection	*redirections;
	int				arg_count;

	redirections = NULL;
	arg_count = 0;
	// Count arguments
	t_token *tmp = current; // for allocation of arg
	while (tmp && tmp->type != PIPE)
	{
		if ((tmp->type == COMMAND || tmp->type == ARGUMENT)
			&& !tmp->is_empty_expansion)
			arg_count++;
		tmp = tmp->next;
	}
	// Allocate args array
	args = malloc((arg_count + 1) * sizeof(char *));
	if (!args)
		return (NULL);
	// Fill arrays and build redirections
	current = fill_arrays(current, args, &redirections);
	// Add command to list
	add_cmd_node(cmd_list, create_cmd_node(args, redirections));
	if (current && current->type == PIPE)
		current = current->next;
	return (current);
}

// main func of build command
t_command	*parse_commands(t_token *tokens)
{
	t_command	*cmd_list;
	t_token		*current;

	cmd_list = NULL;
	current = tokens;
	while (current)
	{
		current = process_command_node(current, &cmd_list);
		if (!current && cmd_list == NULL)
			return (NULL);
	}
	return (cmd_list);
}
