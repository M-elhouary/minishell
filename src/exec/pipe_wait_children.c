/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_wait_children.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hayabusa <hayabusa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 01:26:07 by houardi           #+#    #+#             */
/*   Updated: 2025/08/25 08:23:43 by hayabusa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	wait_checksig(pid_t pid, int *got_sig, int i, int cmd_count)
{
	int	status;
	int	last_cmd_exit;

	last_cmd_exit = 0;
	if (waitpid(pid, &status, 0) == -1)
	{
		perror("waitpid");
		return (1);
	}
	if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGINT)
			*got_sig = 130;
		else if (WTERMSIG(status) == SIGQUIT)
			*got_sig = 131;
	}
	if (i == cmd_count - 1)
		last_cmd_exit = exit_status(status);
	return (last_cmd_exit);
}

void	handle_pipe_signals(int got_sig, int last_cmd_exit)
{
	if (got_sig == 130)
	{
		g_last_signal = SIGINT;
		print("\n", 1);
	}
	else if (got_sig == 131 && last_cmd_exit == 131)
	{
		g_last_signal = SIGQUIT;
		print("Quit (core dumped)\n", 1);
	}
	else
		g_last_signal = 0;
}

int	wait_children(pid_t *pids, int cmd_count)
{
	int	i;
	int	last_cmd_exit;
	int	got_sig;

	last_cmd_exit = 0;
	got_sig = 0;
	i = -1;
	while (++i < cmd_count)
		last_cmd_exit = wait_checksig(pids[i], &got_sig, i, cmd_count);
	handle_pipe_signals(got_sig, last_cmd_exit);
	return (last_cmd_exit);
}
