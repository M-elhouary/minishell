/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_command.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houa <mel-houa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 00:00:00 by mel-houa          #+#    #+#             */
/*   Updated: 2025/07/31 22:48:41 by mel-houa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_command	*create_cmd_node(char **args, char **infile, char **outfile,
		int append, int heredoc)
{
	t_command	*new_cmd;

	new_cmd = (t_command *)malloc(sizeof(t_command));
	if (!new_cmd)
		return (NULL);
	new_cmd->args = args;
	new_cmd->infile = infile;
	new_cmd->outfile = outfile;
	new_cmd->append = append;
	new_cmd->heredoc = heredoc;
	new_cmd->next = NULL;
	return (new_cmd);
}

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

static void	count_tokens(t_token *temp, int *arg_count, int *in_count,
		int *out_count)
{
	while (temp && temp->type != PIPE)
	{
		if ((temp->type == COMMAND || temp->type == ARGUMENT) 
			&& !temp->is_empty_expansion)
			(*arg_count)++;
		else if (temp->type == REDIR_IN || temp->type == HEREDOC)
			(*in_count)++;
		else if (temp->type == REDIR_OUT || temp->type == REDIR_APPEND)
			(*out_count)++;
		temp = temp->next;
	}
}

static int	allocate_arrays(char ***args, char ***infiles, char ***outfiles,
		int arg_count, int in_count, int out_count)
{
	*args = malloc((arg_count + 1) * sizeof(char *));
	*infiles = malloc((in_count + 1) * sizeof(char *));
	*outfiles = malloc((out_count + 1) * sizeof(char *));
	if (!*args || !*infiles || !*outfiles)
		return (0);
	return (1);
}

static t_token	*fill_arrays(t_token *current, char **args, char **infiles,
		char **outfiles)
{
	int	arg_count;
	int	in_count;
	int	out_count;

	arg_count = 0;
	in_count = 0;
	out_count = 0;
	while (current && current->type != PIPE)
	{
		if ((current->type == COMMAND || current->type == ARGUMENT)
			&& !current->is_empty_expansion)
			args[arg_count++] = ft_strdup(current->value);
		else if (current->type == REDIR_IN || current->type == HEREDOC)
		{
			current = current->next;
			if (current)
				infiles[in_count++] = ft_strdup(current->value);
		}
		else if (current->type == REDIR_OUT || current->type == REDIR_APPEND)
		{
			current = current->next;
			if (current)
				outfiles[out_count++] = ft_strdup(current->value);
		}
		current = current->next;
	}
	args[arg_count] = NULL;
	infiles[in_count] = NULL;
	outfiles[out_count] = NULL;
	return (current);
}

static int	check_flags(t_token *temp, int *append_flag, int *heredoc_flag)
{
	while (temp && temp->type != PIPE)
	{
		if (temp->type == REDIR_APPEND)
			*append_flag = 1;
		else if (temp->type == HEREDOC)
			*heredoc_flag = 1;
		temp = temp->next;
	}
	return (1);
}

t_command	*parse_commands(t_token *tokens)
{
	t_command	*cmd_list;
	char		**args;
	char		**infiles;
	char		**outfiles;
	int			counts[3];
	int			flags[2];
	t_token		*current;

	cmd_list = NULL;
	current = tokens;
	while (current)
	{
		counts[0] = 0;
		counts[1] = 0;
		counts[2] = 0;
		flags[0] = 0;
		flags[1] = 0;
		count_tokens(current, &counts[0], &counts[1], &counts[2]);
		check_flags(current, &flags[0], &flags[1]);
		if (!allocate_arrays(&args, &infiles, &outfiles,
				counts[0], counts[1], counts[2]))
			return (NULL);
		current = fill_arrays(current, args, infiles, outfiles);
		add_cmd_node(&cmd_list, create_cmd_node(args, infiles,
				outfiles, flags[0], flags[1]));
		if (current && current->type == PIPE)
			current = current->next;
	}
	return (cmd_list);
}

static void	free_string_array(char **array)
{
	int	i;

	if (!array)
		return ;
	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

void	free_commands(t_command *commands)
{
	t_command	*current;
	t_command	*next;

	current = commands;
	while (current)
	{
		next = current->next;
		free_string_array(current->args);
		free_string_array(current->infile);
		free_string_array(current->outfile);
		free(current);
		current = next;
	}
}