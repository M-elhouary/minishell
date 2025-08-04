/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builin.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: houardi <houardi@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 03:57:01 by houardi           #+#    #+#             */
/*   Updated: 2025/08/03 23:27:50 by houardi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_builtin	exec_builtin(t_command *cmd, t_env **env)
{
	if (!cmd || !cmd->args || !cmd->args[0])
		return (NOT_BUILTIN);
	if (ft_strcmp(cmd->args[0], "echo") == 0)
		return (echo_c(cmd->args));
	else if (ft_strcmp(cmd->args[0], "cd") == 0)
		return (cd_c(cmd->args, env));
	else if (ft_strcmp(cmd->args[0], "pwd") == 0)
		return (pwd_c());
	else if (ft_strcmp(cmd->args[0], "export") == 0)
		return (export_c(cmd->args, env));
	else if (ft_strcmp(cmd->args[0], "unset") == 0)
		return (unset_c(cmd->args, env));
	else if (ft_strcmp(cmd->args[0], "env") == 0)
		return (env_c(*env));
	else if (ft_strcmp(cmd->args[0], "exit") == 0)
		return (exit_c(cmd->args));
	return (NOT_BUILTIN);
}
