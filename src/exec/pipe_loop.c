/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_loop.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houa <mel-houa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 13:18:32 by houardi           #+#    #+#             */
/*   Updated: 2025/08/27 22:50:39 by mel-houa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	cleanup_failure(t_pipe_state *state, pid_t *pids)
{
	int	i;

	if (!state->pipe_created)
	{
		close(state->current_pipe[0]);
		close(state->current_pipe[1]);
	}
	if (state->prev_read_fd != -1)
		close(state->prev_read_fd);
	i = 0;
	while (i < state->cmd_index)
	{
		if (pids[i] > 0)
			kill(pids[i], SIGTERM);
		i++;
	}
	i = -1;
	while (++i < state->cmd_index)
		waitpid(pids[i], NULL, 0);
	return (-1);
}

void	run_child_process(t_command *cmd, t_pipe_state *state,
		t_env **env, int is_last)
{
	int	res;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (state->cmd_index > 0 && state->prev_read_fd != -1)
		dup2(state->prev_read_fd, STDIN_FILENO);
	if (!is_last)
		dup2(state->current_pipe[1], STDOUT_FILENO);
	if (state->prev_read_fd != -1)
		close(state->prev_read_fd);
	if (!is_last)
	{
		close(state->current_pipe[0]);
		close(state->current_pipe[1]);
	}
	res = exec_cmd(cmd, env, STDOUT_FILENO, 1);
	clean_all();
	exit(res);
}

int	create_child_process(t_command *current_cmd, t_pipe_state *state,
		t_env **env, pid_t *pids)
{
	int	is_last;

	is_last = (state->cmd_index == state->total_cmds - 1);
	pids[state->cmd_index] = fork();
	if (pids[state->cmd_index] == -1)
	{
		perror("fork");
		return (-1);
	}
	if (pids[state->cmd_index] == 0)
		run_child_process(current_cmd, state, env, is_last);
	return (0);
}

void	parent_after_fork_close_update(t_pipe_state *state)
{
	if (state->prev_read_fd != -1)
		close(state->prev_read_fd);
	if (!state->pipe_created)
	{
		close(state->current_pipe[1]);
		state->prev_read_fd = state->current_pipe[0];
	}
	else
		state->prev_read_fd = -1;
}

int	pipe_loop(t_command *cmd_list, t_env **env, pid_t *pids, int cmd_count)
{
	t_command		*curr_cmd;
	t_pipe_state	state;

	curr_cmd = cmd_list;
	state.cmd_index = 0;
	state.total_cmds = cmd_count;
	state.prev_read_fd = -1;
	while (curr_cmd && state.cmd_index < cmd_count)
	{
		state.pipe_created = (state.cmd_index == cmd_count - 1);
		if (open_next_pipe(state.pipe_created, state.current_pipe) == -1)
			return (cleanup_failure(&state, pids));
		if (create_child_process(curr_cmd, &state, env, pids) == -1)
			return (cleanup_failure(&state, pids));
		parent_after_fork_close_update(&state);
		curr_cmd = curr_cmd->next;
		state.cmd_index++;
	}
	return (state.cmd_index);
}
