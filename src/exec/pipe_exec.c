/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_exec.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hayabusa <hayabusa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 03:51:06 by houardi           #+#    #+#             */
/*   Updated: 2025/08/25 09:54:44 by hayabusa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


// PATH: permission
// cat | cat | cat;
//	exit staus heredoc
//  .
// ..

// minishell$ cat << "$US"E"R"
// > ^C
// "$US"E"R": No such file or directory

// minishell$ exit | ls
// exit
// 1   456  fil8  file8    libft     minishell  src
// 45  78   file  include  Makefile  README.md

// minishell$ cat
// ^C

// minishell$ minishell$ minishell$ 


// houardi@c1r1p14:~/Desktop/minish__/minish$ cat | cat
// ^\Quit (core dumped)

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
	pv.exit_status = 0;
	if (pv.cmd_count == 1)
	{
		cmd_list->print_exit = 1;
		return (exec_cmd(cmd_list, env, STDOUT_FILENO, 0));
	}
	cmd_list->status_exit = 0;
	g_last_signal = 0;
	if (ignore_sig(&pv) == -1)
		return (1);
	if (run_wait_pipe(&pv) == -1)
		return (1);
	free(pv.pids);
	restore_sig(&pv);
	return (pv.exit_status);
}


// declare -x _="/goinfre/mel-houa/cur42/minishell/./minishell" MATBA3HAX ALA7YA

// minishell$ export  hhhhh
// minishell$ export 
// bla=bla
// hhhhh
// [Exit code: 1] PRINTI TAHADI ALAHYA


// [Exit code: 127] WALAHYA PRINTI HADI F STDERROR NOT OUT




// minishell$ << "$USER"
// >$USER
// [Exit code: 1]

// minishell$ << o
// >o
// 0
// [Exit code: 1] if exit code is 0 why changing it when you do nothing ala7ya matb9ax t7arf


// minishell$ << o | ls
// >o
// 1
// 1
// include  libft  Makefile  minishell  README.md  src  test_cases.txt
// hadi dyali ana wyak fiha

