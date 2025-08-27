/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_exec.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: houardi <houardi@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 03:51:06 by houardi           #+#    #+#             */
/*   Updated: 2025/08/27 02:51:05 by houardi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t g_last_signal = 0;

int	ignore_sig(t_pipe_var *pv)
{
	struct sigaction	sa;
	
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sa.sa_handler = SIG_IGN;
	if (sigaction(SIGINT, &sa, &pv->old_int) == -1)
		return (-1);
	if (sigaction(SIGQUIT, &sa, &pv->old_quit) == -1)
	{
		sigaction(SIGINT, &pv->old_int, NULL);
		return (-1);
	}
	return (0);
}

void	restore_sig(t_pipe_var *pv)
{
	sigaction(SIGINT, &pv->old_int, NULL);
	sigaction(SIGQUIT, &pv->old_quit, NULL);
}

int	run_wait_pipe(t_pipe_var *pv)
{
	pv->pids = malloc(sizeof(pid_t) * pv->cmd_count);
	if (!pv->pids)
	{
		// restore_sig(pv);
		return (-1);
	}
	pv->procs_created = pipe_loop(pv->cmd_list, pv->env, pv->pids, pv->cmd_count);
	if (pv->procs_created == -1)
	{
		free(pv->pids);
		restore_sig(pv);
		return (-1);
	}
	pv->exit_status = wait_children(pv->pids, pv->procs_created);
	return (0);
}

int	exec_pipe(t_command *cmd_list, t_env **env)
{
	t_pipe_var	pv;

	pv.cmd_list = cmd_list;
	pv.env = env;
	pv.cmd_count = count_cmds(cmd_list);
	pv.pids = NULL;
	pv.procs_created = 0;
	// pv.exit_status = 0;
	if (pv.cmd_count == 1)
	{
		cmd_list->print_exit = 1;
		return (exec_cmd(cmd_list, env, STDOUT_FILENO, 0));
	}
	// cmd_list->status_exit = 0;
	g_last_signal = 0;
	if (ignore_sig(&pv) == -1)
		return (1);
	if (run_wait_pipe(&pv) == -1)
		return (1);
	free(pv.pids);
	restore_sig(&pv);
	return (pv.exit_status);
}
