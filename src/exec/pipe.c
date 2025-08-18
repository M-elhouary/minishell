/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: houardi <houardi@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 03:51:06 by houardi           #+#    #+#             */
/*   Updated: 2025/08/18 03:47:02 by houardi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


// PATH: permission
// cat | cat | cat;
//	exit staus heredoc
// .
// ..

#include "minishell.h"

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

int	create_pipes(int ***pipes, int cmd_count)
{
	int	i;
	
	*pipes = malloc(sizeof(int *) * (cmd_count - 1));
	if (!*pipes)
		return (0);
	i = 0;
	while (i < cmd_count - 1)
	{
		(*pipes)[i] = malloc(sizeof(int) * 2);
		if (!(*pipes)[i] || pipe((*pipes)[i]) == -1)
		{
			perror("pipe");
			while (--i >= 0)
			{
				close((*pipes)[i][0]);
				close((*pipes)[i][1]);
				free((*pipes)[i]);
			}
			free(*pipes);
			return (0);
		}
		i++;
	}
	return (1);
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

int	setup_child_pipes(int **pipes, int cmd_count, int i, int prev_pipe_read)
{
	(void)prev_pipe_read;
	if (i > 0)
		dup2(pipes[i - 1][0], STDIN_FILENO);
	if (i < cmd_count - 1)
		dup2(pipes[i][1], STDOUT_FILENO);
	int j = 0;
	while (j < cmd_count - 1)
	{
		close(pipes[j][0]);
		close(pipes[j][1]);
		j++;
	}
	return (0);
}

void	handle_child_process(t_command *current_cmd, int **pipes, int cmd_count, int i, t_env **env)
{
	// In child: default signal behavior so Ctrl-C/\ quit affect the pipeline
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);

	// Set up stdin/stdout for this child from the pipeline
	setup_child_pipes(pipes, cmd_count, i, -1);

	// Delegate execution (builtins, redirs, external) to exec_cmd_ex in child mode
	// Pass STDOUT_FILENO since stdout is already wired to the pipe
	int status = exec_cmd_ex(current_cmd, env, STDOUT_FILENO, 1);
	exit(status);
}

int	create_child_process(t_command *current_cmd, int **pipes, int cmd_count, int i, t_env **env, pid_t *pids)
{
	pids[i] = fork();
	if (pids[i] == -1)
	{
		perror("fork");
		return (-1);
	}
	if (pids[i] == 0)
		handle_child_process(current_cmd, pipes, cmd_count, i, env);
	
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
		// For all other commands, we don't care about their exit status
		i++;
	}
	// If pipeline was interrupted by Ctrl-C, print a newline so next prompt is on a new line
	if (interrupted_by_sigint)
		print("\n", 1);
	return (last_cmd_exit);
}

int	*allocate_pipeline_resources(int cmd_count, pid_t **pids)
{
	*pids = malloc(sizeof(pid_t) * cmd_count);
	if (!*pids)
		return (NULL);
	return (malloc(1)); // Dummy return, pipes allocated separately
}

void	cleanup_pipeline_resources(pid_t *pids, int **pipes, int cmd_count)
{
	int i = 0;
	while (i < cmd_count - 1)
	{
		free(pipes[i]);
		i++;
	}
	free(pipes);
	free(pids);
}

int	execute_pipeline_loop(t_command *cmd_list, t_env **env, pid_t *pids, int **pipes, int cmd_count)
{
	t_command	*current_cmd;
	int			i;

	current_cmd = cmd_list;
	i = 0;
	
	// Create all pipes first
	while (i < cmd_count - 1)
	{
		if (pipe(pipes[i]) == -1)
		{
			perror("pipe");
			return (-1);
		}
		i++;
	}
	
	// Fork all processes
	current_cmd = cmd_list;
	i = 0;
	while (current_cmd && i < cmd_count)
	{
		if (create_child_process(current_cmd, pipes, cmd_count, i, env, pids) == -1)
			return (-1);
		current_cmd = current_cmd->next;
		i++;
	}
	
	// Close all pipes in parent
	i = 0;
	while (i < cmd_count - 1)
	{
		close(pipes[i][0]);
		close(pipes[i][1]);
		i++;
	}
	
	return (i); // Return number of processes created
}

int	exec_pipeline(t_command *cmd_list, t_env **env)
{
	pid_t		*pids;
	int			**pipes;
	int			cmd_count;
	int			exit_status;
	int			processes_created;
	int			i;
	void		(*old_int)(int);
	void		(*old_quit)(int);

	cmd_count = count_cmds(cmd_list);
	// printf("%d\n", cmd_count);
	if (cmd_count == 1)
		return (exec_cmd(cmd_list, env, STDOUT_FILENO));

	// Parent: temporarily ignore SIGINT/SIGQUIT while running foreground pipeline
	old_int = signal(SIGINT, SIG_IGN);
	old_quit = signal(SIGQUIT, SIG_IGN);

	pids = malloc(sizeof(pid_t) * cmd_count);
	pipes = malloc(sizeof(int *) * (cmd_count - 1));
	if (!pids || !pipes)
	{
		free(pids);
		free(pipes);
		// Restore parent signal handlers before returning
		signal(SIGINT, old_int);
		signal(SIGQUIT, old_quit);
		return (1);
	}
	
	// Allocate individual pipes
	i = 0;
	while (i < cmd_count - 1)
	{
		pipes[i] = malloc(sizeof(int) * 2);
		if (!pipes[i])
		{
			while (--i >= 0)
				free(pipes[i]);
			free(pipes);
			free(pids);
			// Restore parent signal handlers before returning
			signal(SIGINT, old_int);
			signal(SIGQUIT, old_quit);
			return (1);
		}
		i++;
	}
	
	processes_created = execute_pipeline_loop(cmd_list, env, pids, pipes, cmd_count);
	if (processes_created == -1)
	{
		cleanup_pipeline_resources(pids, pipes, cmd_count);
		// Restore handlers before returning
		signal(SIGINT, old_int);
		signal(SIGQUIT, old_quit);
		return (1);
	}
	
	exit_status = wait_for_children(pids, processes_created);
	cleanup_pipeline_resources(pids, pipes, cmd_count);

	// Restore parent's original signal handlers
	signal(SIGINT, old_int);
	signal(SIGQUIT, old_quit);
	
	return (exit_status);
}
