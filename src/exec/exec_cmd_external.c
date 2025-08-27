/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd_external.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: houardi <houardi@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 10:52:09 by houardi           #+#    #+#             */
/*   Updated: 2025/08/26 10:52:10 by houardi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exec_child(t_command *cmd, t_env **env)
{
	char	**env_arr;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	env_arr = env_to_array(*env);
	if (!env_arr)
	{
		fprintf(stderr, "env err\n");
		exit(1);
	}
	execve(cmd->path, cmd->args, env_arr);
	free_env_array(env_arr);
	if (errno == ENOEXEC)
	{
		fprintf(stderr, "minishell: %s: cannot execute file\n", cmd->args[0]);
		exit (126);
	}
	else
	{
		perror("execve");
		exit(127);
	}
}

int	wait_child(pid_t pid)
{
    int	status;

    while (waitpid(pid, &status, 0) == -1)
    {
        if (errno == EINTR)
            continue;
        perror("waitpid");
        return (1);
    }
    // For a single command, print one newline on Ctrl-C
    if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
    {
        g_last_signal = SIGINT;
        write(STDOUT_FILENO, "\n", 1);
    }
    else
        g_last_signal = 0;
    return (exit_status(status));
}

void	setup_child_signals_and_exec(t_command *cmd, t_env **env, int fd)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (handle_redirections(cmd->redirections) != 0)
		exit(1);
	if (fd != STDOUT_FILENO)
		dup2(fd, STDOUT_FILENO);
	exec_child(cmd, env);
}

int	fork_and_exec(t_command *cmd, t_env **env, int fd)
{
	pid_t pid;
	void (*old_int)(int);
	void (*old_quit)(int);
	int rc;

	old_int = signal(SIGINT, SIG_IGN);
	old_quit = signal(SIGQUIT, SIG_IGN);
	g_last_signal = 0;

	pid = fork();
	if (pid == -1)
	{
		signal(SIGINT, old_int);
		signal(SIGQUIT, old_quit);
		perror("fork");
		return (1);
	}
	if (pid == 0)
		setup_child_signals_and_exec(cmd, env, fd);
	rc = wait_child(pid);
	signal(SIGINT, old_int);
	signal(SIGQUIT, old_quit);
	return (rc);
}

int	handle_external_command_mode(t_command *cmd, t_env **env, int fd, int in_child)
{
	int	validate_res;

	if (check_cmd_path(cmd->path, cmd->args[0]))
		return (127);
	validate_res = validate_cmd_path(cmd->path, cmd->args[0]);
	if (validate_res != 0)
		return (validate_res);

	if (in_child)
		exec_child(cmd, env);

	return (fork_and_exec(cmd, env, fd));
}
