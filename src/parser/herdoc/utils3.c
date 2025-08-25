/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houa <mel-houa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 10:28:32 by mel-houa          #+#    #+#             */
/*   Updated: 2025/08/25 06:58:22 by mel-houa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	g_herdoc_fd = -1;

void	heredoc_cleanup_handler(int sig)
{
	(void)sig;
	write(STDOUT_FILENO, "\n", 1);
	if (g_herdoc_fd >= 0)
	{
		close(g_herdoc_fd);
		g_herdoc_fd = -1;
	}
	exit(130);
}

static int	signal_and_delimiter(char *delimiter, int fd,
		char **clean_delimiter, t_gc *gc)
{
	int	quotes_for_expansion;

	g_herdoc_fd = fd;
	signal(SIGINT, heredoc_cleanup_handler);
	if (!prepare_delimiter_gc(clean_delimiter, delimiter, &quotes_for_expansion,
			gc))
	{
		close(fd);
		g_herdoc_fd = -1;
		free(delimiter);
		return (-1);
	}
	return (quotes_for_expansion);
}

static void	heredoc_cleanup(char *clean_delimiter, char *delimiter, int fd)
{
	free(clean_delimiter);
	free(delimiter);
	close(fd);
	g_herdoc_fd = -1;
}

void	similation_herdoc(char *delimiter, int fd, t_heredoc_params *params)
{
	t_heredoc_ctx	ctx;
	char			*clean_delimiter;
	int				quotes_for_expansion;

	ctx = (t_heredoc_ctx){delimiter, fd, params->env_list, params->cmd,
		params->gc};
	quotes_for_expansion = signal_and_delimiter(delimiter, fd, &clean_delimiter,
			ctx.gc);
	if (quotes_for_expansion == -1)
		return ;
	heredoc_readline_loop(&ctx, clean_delimiter, quotes_for_expansion);
	heredoc_cleanup(clean_delimiter, delimiter, fd);
}

int	execute_heredoc_child(t_token *tmp, int fd, t_heredoc_params *params)
{
	int	pid;
	int	wait_result;
	int	status;

	pid = fork();
	if (pid == 0)
	{
		similation_herdoc(gc_strdup(params->gc, tmp->next->value), fd, params);
		close(fd);
		exit(0);
	}
	wait_result = waitpid(pid, &status, 0);
	close(fd);
	if (wait_result == -1)
		return (0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (0);
}
