/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houa <mel-houa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 03:51:06 by houardi           #+#    #+#             */
/*   Updated: 2025/08/17 03:01:52 by mel-houa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

int	exec_pipeline(t_command *cmd_list, t_env **env)
{
	int			cmd_count;
	int			**pipes;
	pid_t		*pids;
	t_command	*current;
	int			i;
	int			status;
	int			last_exit;

	cmd_count = count_cmds(cmd_list);
	
	// Single command - just use exec_cmd
	if (cmd_count == 1)
		return (exec_cmd(cmd_list, env, STDOUT_FILENO));
	
	// Multiple commands - create pipes
	if (!create_pipes(&pipes, cmd_count))
		return (1);
	
	// Allocate pids array
	pids = malloc(sizeof(pid_t) * cmd_count);
	if (!pids)
	{
		cleanup_pipes(pipes, cmd_count);
		return (1);
	}
	
	// Fork each command
	current = cmd_list;
	i = 0;
	while (current && i < cmd_count)
	{
		pids[i] = fork();
		if (pids[i] == -1)
		{
			perror("fork");
			break;
		}
		else if (pids[i] == 0)
		{
			// Child process - setup pipes and call exec_cmd
			
			// Redirect input from previous pipe
			if (i > 0)
				dup2(pipes[i - 1][0], STDIN_FILENO);
			
			// Redirect output to next pipe  
			if (i < cmd_count - 1)
				dup2(pipes[i][1], STDOUT_FILENO);
			
			// Close all pipe file descriptors
			close_all_pipes(pipes, cmd_count);
			
			// Use exec_cmd - it will handle everything (builtins, external commands, etc.)
			exit(exec_cmd(current, env, STDOUT_FILENO));
		}
		current = current->next;
		i++;
	}
	
	// Parent: close all pipes
	close_all_pipes(pipes, cmd_count);
	
	// Wait for all children
	last_exit = 0;
	i = 0;
	while (i < cmd_count)
	{
		if (waitpid(pids[i], &status, 0) == -1)
			perror("waitpid");
		else if (i == cmd_count - 1)  // Last command's exit status
			last_exit = exit_status(status);
		else if (WIFSIGNALED(status))
		{
			if(WTERMSIG(status) == SIGINT)
			{
				
				write(STDERR_FILENO, "\n", 1);
				return (last_exit = 130);
			}
				
			if (WTERMSIG(status) == SIGQUIT)
			{
				
				write(STDERR_FILENO, "Quit (core dumped)\n", 19);
				return (last_exit = 131);
			}
		}
		i++;
	}
	
	// Cleanup
	cleanup_pipes(pipes, cmd_count);
	free(pids);
	
	return (last_exit);
}
