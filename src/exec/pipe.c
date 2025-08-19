/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houa <mel-houa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 03:51:06 by houardi           #+#    #+#             */
/*   Updated: 2025/08/19 02:37:39 by mel-houa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


// PATH: permission
// cat | cat | cat;
//	exit staus heredoc
// .
// ..

// minishell$ cat << "$US"E"R"
// > ^C
// "$US"E"R": No such file or directory

// minishell$ exit | ls
// exit
// 1   456  fil8  file8    libft     minishell  src
// 45  78   file  include  Makefile  README.md

#include "minishell.h"

/* Count commands in pipeline */
int	count_cmds(t_command *cmd)
{
	int	count;

	count = 0;
	while (cmd)
	{
		count++;
		cmd = cmd->next;
	}
	return (count);
}

void	close_all_pipes(int **pipes, int cmd_count)
{
	int	i;

	i = 0;
	while (i < cmd_count - 1)
	{
		close(pipes[i][0]);
		close(pipes[i][1]);
		i++;
	}
}

void	cleanup_pipes(int **pipes, int cmd_count)
{
	int	i;

	i = 0;
	while (i < cmd_count - 1)
	{
		free(pipes[i]);
		i++;
	}
	free(pipes);
}

// Set up child's stdin/stdout using only the previous read end and the current pipe
int	setup_child_pipes(int i, int is_last, int prev_read_fd, int curr_pipe[2])
{
	// If not the first command, hook up stdin to previous read end
	if (i > 0 && prev_read_fd != -1)
		dup2(prev_read_fd, STDIN_FILENO);
	// If not the last command, hook up stdout to current pipe's write end
	if (!is_last)
		dup2(curr_pipe[1], STDOUT_FILENO);
	// Close now-unneeded fds in the child
	if (prev_read_fd != -1)
		close(prev_read_fd);
	if (!is_last)
	{
		close(curr_pipe[0]);
		close(curr_pipe[1]);
	}
	return (0);
}

void	handle_child_process(t_command *current_cmd, int i, int is_last, int prev_read_fd, int curr_pipe[2], t_env **env)
{
	// In child: default signal behavior so Ctrl-C/\ quit affect the pipeline
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);

	// Set up stdin/stdout for this child from the pipeline
	setup_child_pipes(i, is_last, prev_read_fd, curr_pipe);

	// Delegate execution (builtins, redirs, external) to exec_cmd_ex in child mode
	// Pass STDOUT_FILENO since stdout is already wired to the pipe
	int status = exec_cmd_ex(current_cmd, env, STDOUT_FILENO, 1);
	exit(status);
}

int	create_child_process(t_command *current_cmd, int i, int is_last, int prev_read_fd, int curr_pipe[2], t_env **env, pid_t *pids)
{
	pids[i] = fork();
	if (pids[i] == -1)
	{
		perror("fork");
		return (-1);
	}
	if (pids[i] == 0)
		handle_child_process(current_cmd, i, is_last, prev_read_fd, curr_pipe, env);
	return (0);
}

int	wait_for_children(pid_t *pids, int cmd_count)
{
	int	i;
	int	status;
	int	last_cmd_exit;
	int	interrupted_by_sigint;

	last_cmd_exit = 0;
	interrupted_by_sigint = 0;
	i = 0;
	while (i < cmd_count)
	{
		waitpid(pids[i], &status, 0);
		if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
			interrupted_by_sigint = 1;
		if (i == cmd_count - 1)
			last_cmd_exit = exit_status(status);
		i++;
	}
	// If pipeline was interrupted by Ctrl-C, print a newline so next prompt is on a new line
	if (interrupted_by_sigint)
		print("\n", 1);
	return (last_cmd_exit);
}

void	cleanup_pipeline_resources(pid_t *pids, int **pipes, int cmd_count)
{
	int i;

	// Free pipes array only if provided
	if (pipes)
	{
		i = 0;
		while (i < cmd_count - 1)
		{
			if (pipes[i])
				free(pipes[i]);
			i++;
		}
		free(pipes);
	}
	if (pids)
		free(pids);
}

// Execute the pipeline creating at most one pipe at a time
int	execute_pipeline_loop(t_command *cmd_list, t_env **env, pid_t *pids, int cmd_count)
{
	t_command	*current_cmd;
	int			i;
	int			prev_read_fd;
	int			curr_pipe[2];
	int			is_last;

	current_cmd = cmd_list;
	i = 0;
	prev_read_fd = -1;
	while (current_cmd && i < cmd_count)
	{
		is_last = (i == cmd_count - 1);
		if (!is_last)
		{
			if (pipe(curr_pipe) == -1)
			{
				perror("pipe");
				if (prev_read_fd != -1)
					close(prev_read_fd);
				return (-1);
			}
		}
		else
		{
			curr_pipe[0] = -1;
			curr_pipe[1] = -1;
		}

		if (create_child_process(current_cmd, i, is_last, prev_read_fd, curr_pipe, env, pids) == -1)
		{
			// On fork failure, close any open fds and stop
			if (!is_last)
			{
				close(curr_pipe[0]);
				close(curr_pipe[1]);
			}
			if (prev_read_fd != -1)
				close(prev_read_fd);
			return (-1);
		}

		// Parent: close fds no longer needed
		if (prev_read_fd != -1)
			close(prev_read_fd);
		if (!is_last)
		{
			close(curr_pipe[1]); // keep only the read end for next cmd
			prev_read_fd = curr_pipe[0];
		}
		else
		{
			prev_read_fd = -1;
		}

		current_cmd = current_cmd->next;
		i++;
	}
	// After forking all, close any remaining read end in parent
	if (prev_read_fd != -1)
		close(prev_read_fd);
	return (i); // number of processes created
}

int	exec_pipeline(t_command *cmd_list, t_env **env)
{
	pid_t		*pids;
	int			cmd_count;
	int			exit_status_code;
	int			processes_created;
	void		(*old_int)(int);
	void		(*old_quit)(int);

	cmd_count = count_cmds(cmd_list);
	if (cmd_count == 1)
		return (exec_cmd(cmd_list, env, STDOUT_FILENO));

	// Parent: temporarily ignore SIGINT/SIGQUIT while running foreground pipeline
	old_int = signal(SIGINT, SIG_IGN);
	old_quit = signal(SIGQUIT, SIG_IGN);

	pids = malloc(sizeof(pid_t) * cmd_count);
	if (!pids)
	{
		// Restore parent signal handlers before returning
		signal(SIGINT, old_int);
		signal(SIGQUIT, old_quit);
		return (1);
	}

	processes_created = execute_pipeline_loop(cmd_list, env, pids, cmd_count);
	if (processes_created == -1)
	{
		cleanup_pipeline_resources(pids, NULL, cmd_count);
		// Restore handlers before returning
		signal(SIGINT, old_int);
		signal(SIGQUIT, old_quit);
		return (1);
	}

	exit_status_code = wait_for_children(pids, processes_created);
	cleanup_pipeline_resources(pids, NULL, cmd_count);

	// Restore parent's original signal handlers
	signal(SIGINT, old_int);
	signal(SIGQUIT, old_quit);

	return (exit_status_code);
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

