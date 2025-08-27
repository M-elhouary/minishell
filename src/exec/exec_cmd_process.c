/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd_process.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: houardi <houardi@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 10:52:16 by houardi           #+#    #+#             */
/*   Updated: 2025/08/27 02:49:31 by houardi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	restore_file_descriptors(int original_stdin, int original_stdout)
{
	dup2(original_stdin, STDIN_FILENO);
	dup2(original_stdout, STDOUT_FILENO);
	close(original_stdin);
	close(original_stdout);
}

int	handle_builtin_execution(t_command *cmd, t_env **env, int original_stdin, int original_stdout)
{
	t_builtin	built_res;
	
	built_res = exec_builtin(cmd, env, STDOUT_FILENO);
	if (built_res != NOT_BUILTIN)
	{
		restore_file_descriptors(original_stdin, original_stdout);
		return (built_res);
	}
	return (NOT_BUILTIN);
}

int	execute_builtin_child_mode(t_command *cmd, t_env **env)
{
	t_builtin built_res;
	
	built_res = exec_builtin(cmd, env, STDOUT_FILENO);
	if (built_res != NOT_BUILTIN)
		return (built_res);
	return (NOT_BUILTIN);
}

int	process_cmd(t_command *cmd, t_env **env, t_exec_var *ev)
{
	int builtin_result;

	if (!ev->in_child)
	{
		builtin_result = handle_builtin_execution(cmd, env, ev->original_stdin, ev->original_stdout);
		if (builtin_result != NOT_BUILTIN)
			return (builtin_result);
		restore_file_descriptors(ev->original_stdin, ev->original_stdout);
	}
	else
	{
		builtin_result = execute_builtin_child_mode(cmd, env);
		if (builtin_result != NOT_BUILTIN)
			return (builtin_result);
	}
	return (handle_external_command_mode(cmd, env, ev->fd, ev->in_child));
}