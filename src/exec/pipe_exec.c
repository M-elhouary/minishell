/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_exec.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houa <mel-houa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 03:51:06 by houardi           #+#    #+#             */
/*   Updated: 2025/08/27 22:40:13 by mel-houa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	run_wait_pipe(t_pipe_var *pv)
{
	t_clean	**cleanup;

	cleanup = grepclean();
	pv->pids = gc_malloc((*cleanup)->gc, sizeof(pid_t) * pv->cmd_count);
	if (!pv->pids)
	{
		signal(SIGINT, sigint_interactive);
		return (-1);
	}
	pv->procs_created = pipe_loop(pv->cmd_list,
			pv->env, pv->pids, pv->cmd_count);
	if (pv->procs_created == -1)
	{
		signal(SIGINT, sigint_interactive);
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
	if (pv.cmd_count == 1)
	{
		cmd_list->print_exit = 1;
		return (exec_cmd(cmd_list, env, STDOUT_FILENO, 0));
	}
	g_last_signal = 0;
	signal(SIGINT, SIG_IGN);
	if (run_wait_pipe(&pv) == -1)
		return (1);
	signal(SIGINT, sigint_interactive);
	return (pv.exit_status);
}
