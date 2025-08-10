/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_command.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houa <mel-houa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 00:00:00 by mel-houa          #+#    #+#             */
/*   Updated: 2025/08/10 16:02:17 by mel-houa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse.h"



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
static void	add_cmd_node(t_command **head, t_command *new)
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



// static int	allocate_arrays(char ***args, char ***infiles, char ***outfiles, int arg_count, int in_count, int out_count)
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

static t_token	*fill_arrays(t_token *current, char **args, t_redirection **redirections)
{
    int arg_count = 0;
    t_redirection *new_redir;

    while (current && current->type != PIPE)
    {
        if ((current->type == COMMAND || current->type == ARGUMENT)
            && !current->is_empty_expansion)
            args[arg_count++] = ft_strdup(current->value);
        else if (current->type == REDIR_IN)
        {
            current = current->next;
            if (current)
            {
                new_redir = create_redirection(REDIR_TYPE_IN, current->value);
                add_redirection(redirections, new_redir);
            }
        }
        else if (current->type == REDIR_OUT)
        {
            current = current->next;
            if (current)
            {
                new_redir = create_redirection(REDIR_TYPE_OUT, current->value);
                add_redirection(redirections, new_redir);
            }
        }
        else if (current->type == REDIR_APPEND)
        {
            current = current->next;
            if (current)
            {
                new_redir = create_redirection(REDIR_TYPE_APPEND, current->value);
                add_redirection(redirections, new_redir);
            }
        }
        else if (current->type == HEREDOC)
        {
            current = current->next;
            if (current)
            {
                new_redir = create_redirection(REDIR_TYPE_HEREDOC, current->value);
                add_redirection(redirections, new_redir);
            }
        }
        if (current)
            current = current->next;
    }
    args[arg_count] = NULL;
    return current;
}



// Helper to process a single command in the parse_commands loop
static t_token *process_command_node(t_token *current, t_command **cmd_list)
{
    char **args;
    t_redirection *redirections = NULL;
    int arg_count = 0;
    
    // Count arguments
    t_token *tmp = current;
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
        return NULL;
    
    // Fill arrays and build redirections
    current = fill_arrays(current, args, &redirections);
    
    // Add command to list
    add_cmd_node(cmd_list, create_cmd_node(args, redirections));
    
    if (current && current->type == PIPE)
        current = current->next;
    return current;
}

// main func of build command 
t_command *parse_commands(t_token *tokens)
{
	t_command *cmd_list = NULL;
	t_token *current = tokens;
	while (current)
	{
		current = process_command_node(current, &cmd_list);
		if (!current && cmd_list == NULL)
			return NULL;
	}
	return cmd_list;
}
