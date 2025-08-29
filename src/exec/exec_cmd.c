/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houa <mel-houa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 04:00:44 by houardi           #+#    #+#             */
/*   Updated: 2025/08/27 22:50:47 by mel-houa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	setup_redirections_and_fd(t_command *cmd, int fd,
	int *original_stdin, int *original_stdout)
{
	*original_stdin = dup(STDIN_FILENO);
	*original_stdout = dup(STDOUT_FILENO);
	if (handle_redirections(cmd->redirections) != 0)
	{
		close(*original_stdin);
		close(*original_stdout);
		return (1);
	}
	if (fd != STDOUT_FILENO)
		dup2(fd, STDOUT_FILENO);
	return (0);
}

int	handle_child_mode_setup(t_command *cmd, int fd)
{
	if (handle_redirections(cmd->redirections) != 0)
		return (1);
	if (fd != STDOUT_FILENO)
		dup2(fd, STDOUT_FILENO);
	return (0);
}

int	exec_cmd(t_command *cmd, t_env **env, int fd, int in_child)
{
	t_exec_var	ev;
	int			original_stdin;
	int			original_stdout;

	original_stdin = -1;
	original_stdout = -1;
	if ((!cmd || !cmd->args || !cmd->args[0]) && !cmd->redirections)
		return (1);
	if (!in_child)
	{
		if (setup_redirections_and_fd(cmd, fd, &original_stdin,
				&original_stdout) != 0)
			return (1);
	}
	else if (handle_child_mode_setup(cmd, fd) != 0)
		return (1);
	if (!cmd->args[0])
	{
		if (!in_child)
			restore_file_descriptors(original_stdin, original_stdout);
		return (0);
	}
	ev = (t_exec_var){fd, in_child, original_stdin, original_stdout};
	return (process_cmd(cmd, env, &ev));
}
