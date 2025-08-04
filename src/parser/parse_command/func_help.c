/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   func_help.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houa <mel-houa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 20:02:42 by mel-houa          #+#    #+#             */
/*   Updated: 2025/08/03 23:28:24 by mel-houa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse.h"




void	count_tokens(t_token *temp, int *arg_count, int *in_count, int *out_count)
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

int	check_flags(t_token *temp, int *append_flag, int *heredoc_flag)
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

void	free_string_array(char **array)
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