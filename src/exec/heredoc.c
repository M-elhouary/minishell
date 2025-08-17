/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: houardi <houardi@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 10:10:47 by houardi           #+#    #+#             */
/*   Updated: 2025/08/16 07:06:26 by houardi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_heredoc_redirection(t_redirection *redir)
{
	int	fd;

	if (redir->type != REDIR_TYPE_HEREDOC)
		return (0);
	fd = open(redir->file, O_RDONLY);
	if (fd < 0)
	{
		perror(redir->file);
		return (-1);
	}
	if (dup2(fd, STDIN_FILENO) < 0)
	{
		perror("dup2");
		close(fd);
		return (-1);
	}
	close(fd);
	unlink(redir->file);
	return (0);
}

void	cleanup_heredoc_files(t_redirection *redirections)
{
	t_redirection	*current;

	current = redirections;
	while (current)
	{
		if (current->type == REDIR_TYPE_HEREDOC)
			unlink(current->file);
		current = current->next;
	}
}

int	count_heredocs(t_redirection *redirections)
{
	t_redirection	*current;
	int				count;

	count = 0;
	current = redirections;
	while (current)
	{
		if (current->type == REDIR_TYPE_HEREDOC)
			count++;
		current = current->next;
	}
	return (count);
}