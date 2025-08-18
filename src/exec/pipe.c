/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houa <mel-houa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 03:51:06 by houardi           #+#    #+#             */
/*   Updated: 2025/08/17 02:03:48 by mel-houa         ###   ########.fr       */
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
		signal(SIGINT, SIG_IGN); // parsing edit
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
	//signal(SIGINT, SIG_IGN); IGNORE THE PARENT SIGNALES
	while (i < cmd_count)
	{
		if (waitpid(pids[i], &status, 0) == -1)
			perror("waitpid");
		else if (i == cmd_count - 1)  // Last command's exit status
			last_exit = exit_status(status);		
		else if (WIFSIGNALED(status))
			{
				printf("hskfhk");
				if (WTERMSIG(status) == SIGINT)
				{
					write(2, "\n", 1);
					return (last_exit = 130);
				}
				if (WTERMSIG(status) == SIGQUIT)
				{
					write(2, "Quit (Core dump)\n", 17);
					return (last_exit = 131);
				}
			}
		// here check for the type of the signale 
		// if it is sig int or sig quit 
		// and set the exit status to the prober exit code

		//
		i++;

		
	}
	// if(WIFEXITED(last_exit))
	// 	cmd_list->status_exit = WEXITSTATUS(last_exit);
	// else if (WIFSIGNALED(last_exit))
	// {
	// 	cmd_list->status_exit  = 128 + WTERMSIG(last_exit);
	// 	if (WTERMSIG(last_exit) == SIGINT)
	// 		write(1, "hhhhhhhhhh\n", 10);
	// 	else if (WTERMSIG(last_exit))
	// 		write(1, "Core Dump\n", 10);
	// }
	// // Cleanup
	cleanup_pipes(pipes, cmd_count);
	free(pids);
	
	return (last_exit);
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

