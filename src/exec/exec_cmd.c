/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houa <mel-houa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 04:00:44 by houardi           #+#    #+#             */
/*   Updated: 2025/08/17 03:04:55 by mel-houa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_command	*create_cmd(char **args)
{
	t_command	*cmd;
	int		i;
	
	if (!args || !args[0])
		return (NULL);
	cmd = malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);
	i = 0;
	while (args[i])
		i++;
	cmd->ac = i;
	cmd->args = malloc(sizeof(char *) * (i + 1));
	if (!cmd->args)
		return (free(cmd), NULL);
	i = 0;
	while (args[i])
	{
		cmd->args[i] = ft_strdup(args[i]);
		i++;
	}
	cmd->args[i] = NULL;
	cmd->path = locate_cmd(args[0]);
	return (cmd);
}

int	exit_status(int status)
{
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (status);
}

int	validate_cmd_path(char *path, char *cmd)
{
	struct stat st;
	
	if (access(path, F_OK) != 0)
	{
		printf("minishell: %s: No such file or directory\n", cmd);
		return (127);
	}
	if (stat(path, &st) == 0 && S_ISDIR(st.st_mode))
	{
		printf("minishell: %s: Is a directory\n", cmd);
		return (126);
	}
	if (access(path, X_OK) != 0)
	{
		printf("minishell: %s: Permission denied\n", cmd);
		return (126);
	}
	return (0);
}

void	exec_child(t_command *cmd, t_env **env)
{
	char	**env_arr;

	/* Reset signals to default in child processes */
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	env_arr = env_to_array(*env);
	if (!env_arr)
	{
		printf("env err\n");
		exit(1);
	}
	execve(cmd->path, cmd->args, env_arr);
	if (errno == ENOEXEC)
	{
		printf("minishell: %s: cannot execute binary file\n", cmd->args[0]);
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
	
	if (waitpid(pid, &status, 0) == -1)
	{
		perror("waitpid");
		return (1);
	}
	if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGINT)
			write(1, "\n", 1);
	}
	return (exit_status(status));	
}

int	check_cmd_path(char *path, char *cmd)
{
	if (!path)
	{
		if (ft_strchr(cmd, '/'))
			printf("minishell: %s: No such file or directory\n", cmd);
		else
			printf("minishell: %s: command not found\n", cmd);
		return (127);
	}
	else
		return (0);
}

int	exec_cmd(t_command *cmd, t_env **env, int fd)
{
	pid_t		pid;
	t_builtin	built_res;
	int			validate_res;

	if (!cmd || !cmd->args || !cmd->args[0])
		return (1);
	built_res = exec_builtin(cmd, env, fd);
	if (built_res != NOT_BUILTIN)
		return (built_res);
	if (check_cmd_path(cmd->path, cmd->args[0]))
		return (127);
	validate_res = validate_cmd_path(cmd->path, cmd->args[0]);
	if (validate_res != 0)
		return (validate_res);
	signal(SIGINT, SIG_IGN);///new prompt
	pid = fork();
	if (pid == -1)
		return (perror("fork"), 1);
	if (pid == 0)
		exec_child(cmd, env);
	else
		return (wait_child(pid));
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
	free(cmd);
}
