/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: houardi <houardi@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 10:52:21 by houardi           #+#    #+#             */
/*   Updated: 2025/08/26 10:52:22 by houardi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exit_status(int status)
{
	int sig;

	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
	{
		sig = WTERMSIG(status);
		if (sig == SIGPIPE)
			return 0;
		return (128 + sig);
	}
	return (status);
}

int	check_cmd_path(char *path, char *cmd)
{
	if (!path)
	{
		if (ft_strchr(cmd, '/'))
			fprintf(stderr, "minishell: %s: No such file or directory\n", cmd);
		else
			fprintf(stderr, "minishell: %s: command not found\n", cmd);
		return (127);
	}
	else
		return (0);
}


int	validate_cmd_path(char *path, char *cmd)
{
	struct stat	st;

	if (ft_strcmp(cmd, ".") == 0)
	{
		fprintf(stderr, "minishell: %s: filename argument required\n", cmd);
		return (2);
	}
	if (access(path, F_OK) != 0)
	{
		fprintf(stderr, "minishell: %s: No such file or directory\n", cmd);
		return (127);
	}
	if (stat(path, &st) == 0 && S_ISDIR(st.st_mode))
	{
		fprintf(stderr, "minishell: %s: Is a directory\n", cmd);
		return (126);
	}
	if (access(path, X_OK) != 0)
	{
		fprintf(stderr, "minishell: %s: Permission denied\n", cmd);
		return (126);
	}
	return (0);
}

void	free_cmd(t_command *cmd)
{
	int	i;
	
	if (!cmd)
		return;
	if (cmd->args)
	{
		i = 0;
		while (cmd->args[i])
		{
			free(cmd->args[i]);
			i++;
		}
		free(cmd->args);		
	}
	if (cmd->path)
		free(cmd->path);
	if (cmd->redirections)
		free_redirections(cmd->redirections);
	free(cmd);
}