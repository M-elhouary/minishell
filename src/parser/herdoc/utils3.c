/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-abbo <sel-abbo@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 10:28:32 by mel-houa          #+#    #+#             */
/*   Updated: 2025/08/27 07:17:14 by sel-abbo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"



t_clean **grepclean(void)
{
	static t_clean  *cleanchild = NULL;
	if (!cleanchild)
	{
		cleanchild = malloc(sizeof(t_clean));
		if (!cleanchild)
			exit(1);
		cleanchild->env = NULL;
		cleanchild->cmd = NULL;
		cleanchild->gc = NULL;
	}
	return (&cleanchild);
}



void	heredoc_cleanup_handler(int sig)
{
	int	fd;

	t_clean **cleanchild;
	cleanchild = grepclean();
	(void)sig;
	fd = get_herdoc_fd(-1);
	write(STDOUT_FILENO, "\n", 1);
	cleanup_env((*cleanchild)->env);
	gc_free_all((*cleanchild)->gc);
	free((*cleanchild)->cmd);
	free(*cleanchild);
	if (get_herdoc_fd(-1) >= 0)
		close(fd);
	exit(130);
}

static int	signal_and_delimiter(char *delimiter, int fd,
		char **clean_delimiter, t_gc *gc)
{
	int	quotes_for_expansion;

	signal(SIGINT, heredoc_cleanup_handler);
	if (!prepare_delimiter_gc(clean_delimiter, delimiter, &quotes_for_expansion,
			gc))
	{
		close(fd);
		free(delimiter);
		return (-1);
	}
	return (quotes_for_expansion);
}

static void	heredoc_cleanup(char *clean_delimiter, char *delimiter, int fd)
{
	(void)clean_delimiter;
	(void)delimiter;
	close(fd);
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

	signal(SIGINT, SIG_IGN);
	pid = fork();
	if (pid == 0)
	{
		similation_herdoc(gc_strdup(params->gc, tmp->next->value), fd, params);
		close(fd);
		t_clean **cleanchild;
		cleanchild = grepclean();
		cleanup_env((*cleanchild)->env);
		gc_free_all((*cleanchild)->gc);
		free((*cleanchild)->cmd);
		free(*cleanchild);
		exit(0);
	}
	wait_result = waitpid(pid, &status, 0);
	close(fd);
	if (wait_result == -1)
		return (0);
	if (WIFEXITED(status))
		params->cmd->status_exit = WEXITSTATUS(status);
	return (params->cmd->status_exit);
}
