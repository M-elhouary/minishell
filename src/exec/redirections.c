/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: houardi <houardi@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 01:19:45 by houardi           #+#    #+#             */
/*   Updated: 2025/08/17 03:50:22 by houardi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"




// > out | ls
static int	handle_input_redirection(char *file)
{
	int	fd;

	fd = open(file, O_RDONLY);
	if (fd < 0)
	{
		perror(file);
		return (1);
	}
	dup2(fd, STDIN_FILENO);
	close(fd);
	return (0);
}

static int	handle_output_redirection(char *file)
{
	int	fd;

	fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
	{
		perror(file);
		return (1);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return (0);
}

static int	handle_append_redirection(char *file)
{
	int	fd;

	fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
	{
		perror(file);
		return (1);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return (0);
}

static int	handle_heredoc_redirection(char *file)
{
	int	fd;

	fd = open(file, O_RDONLY);
	// printf("%d\n", fd);
	if (fd < 0)
	{
		perror(file);
		return (1);
	}
	dup2(fd, STDIN_FILENO);
	close(fd);
	return (0);
}

static int	process_single_redirection(t_redirection *redir)
{
	if (redir->type == REDIR_TYPE_IN)
		return (handle_input_redirection(redir->file));
	else if (redir->type == REDIR_TYPE_OUT)
		return (handle_output_redirection(redir->file));
	else if (redir->type == REDIR_TYPE_APPEND)
		return (handle_append_redirection(redir->file));
	else if (redir->type == REDIR_TYPE_HEREDOC)
		return (handle_heredoc_redirection(redir->file));
	return (0);
}

int	handle_redirections(t_redirection *redirections)
{
	t_redirection	*current;

	current = redirections;
	while (current)
	{
		if (process_single_redirection(current) != 0)
			return (1);
		current = current->next;
	}
	return (0);
}
