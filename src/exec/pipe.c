/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houa <mel-houa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 03:51:06 by houardi           #+#    #+#             */
/*   Updated: 2025/08/18 20:29:31 by mel-houa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	int j;
	
	(void)prev_pipe_read;
	if (i > 0)
		dup2(pipes[i - 1][0], STDIN_FILENO);
	if (i < cmd_count - 1)
		dup2(pipes[i][1], STDOUT_FILENO);
	j = 0;
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
	char		**env_arr;
	t_builtin	builtin_result;
	
	setup_child_pipes(pipes, cmd_count, i, -1);
	if (current_cmd->redirections)
	{
		if (handle_redirections(current_cmd->redirections) != 0)
			exit(1);
	}
	if (!current_cmd->path)
	{
		if (ft_strchr(current_cmd->args[0], '/'))
		{
			print("minishell: ", 2);
			print(current_cmd->args[0], 2);
			print(": No such file or directory\n", 2);
		}
		else
		{
			print("minishell: ", 2);
			print(current_cmd->args[0], 2);
			print(": command not found\n", 2);
		}
		exit(127);
	}
	builtin_result = exec_builtin(current_cmd, env, STDOUT_FILENO);
	if (builtin_result != NOT_BUILTIN)
		exit(builtin_result);
	env_arr = env_to_array(*env);
	if (!env_arr)
	{
		print("env error\n", 2);
		exit(1);
	}
	execve(current_cmd->path, current_cmd->args, env_arr);
	free_env_array(env_arr);
	perror("execve");
	exit(127);
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

	last_cmd_exit = 0;
	i = 0;
	while (i < cmd_count)
	{
		waitpid(pids[i], &status, 0);
		if (i == cmd_count - 1)
			last_cmd_exit = exit_status(status);
		i++;
	}
	
	return (last_cmd_exit);
}

void	cleanup_pipeline_resources(pid_t *pids, int **pipes, int cmd_count)
{
	int i;
	
	i = 0;
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

	cmd_count = count_cmds(cmd_list);
	if (cmd_count == 1)
		return (exec_cmd(cmd_list, env, STDOUT_FILENO));

	pids = malloc(sizeof(pid_t) * cmd_count);
	pipes = malloc(sizeof(int *) * (cmd_count - 1));
	if (!pids || !pipes)
	{
		free(pids);
		free(pipes);
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
			return (1);
		}
		i++;
	}
	
	processes_created = execute_pipeline_loop(cmd_list, env, pids, pipes, cmd_count);
	if (processes_created == -1)
	{
		cleanup_pipeline_resources(pids, pipes, cmd_count);
		return (1);
	}
	
	exit_status = wait_for_children(pids, processes_created);
	cleanup_pipeline_resources(pids, pipes, cmd_count);
	
	return (exit_status);
}
