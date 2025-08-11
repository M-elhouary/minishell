/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builin.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: houardi <houardi@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 03:57:01 by houardi           #+#    #+#             */
/*   Updated: 2025/08/10 20:19:19 by houardi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_builtin	exec_builtin(t_command *cmd, t_env **env, int fd)
{
	if (!cmd || !cmd->args || !cmd->args[0])
		return (NOT_BUILTIN);
	if (ft_strcmp(cmd->args[0], "echo") == 0)
		return (echo_c(cmd->args, fd));
	else if (ft_strcmp(cmd->args[0], "cd") == 0)
		return (cd_c(cmd->args, env, fd));
	else if (ft_strcmp(cmd->args[0], "pwd") == 0)
		return (pwd_c(fd));
	else if (ft_strcmp(cmd->args[0], "export") == 0)
		return (export_c(cmd->args, env, fd));
	else if (ft_strcmp(cmd->args[0], "unset") == 0)
		return (unset_c(cmd->args, env, fd));
	else if (ft_strcmp(cmd->args[0], "env") == 0)
		return (env_c(*env, fd));
	else if (ft_strcmp(cmd->args[0], "exit") == 0)
		return (exit_c(cmd->args, fd));
	return (NOT_BUILTIN);
}
