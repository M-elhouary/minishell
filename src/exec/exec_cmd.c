/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: houardi <houardi@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 04:00:44 by houardi           #+#    #+#             */
/*   Updated: 2025/08/19 02:50:22 by houardi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
//correct code 
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
// yes 
int	validate_cmd_path(char *path, char *cmd)
{
	struct stat st;
	
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

void	exec_child(t_command *cmd, t_env **env)
{
	char	**env_arr;

	/* Reset signals to default in child processes */
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
		fprintf(stderr, "minishell: %s: cannot execute binary file\n", cmd->args[0]);
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
	return (exit_status(status));
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

static int	setup_redirections_and_fd(t_command *cmd, int fd, int *original_stdin, int *original_stdout)
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

static void	restore_file_descriptors(int original_stdin, int original_stdout)
{
	dup2(original_stdin, STDIN_FILENO);
	dup2(original_stdout, STDOUT_FILENO);
	close(original_stdin);
	close(original_stdout);
}

static int	handle_builtin_execution(t_command *cmd, t_env **env, int original_stdin, int original_stdout)
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

static int	handle_external_command_mode(t_command *cmd, t_env **env, int fd, int in_child)
{
    pid_t   pid;
    int     validate_res;

    if (check_cmd_path(cmd->path, cmd->args[0]))
        return (127);
    validate_res = validate_cmd_path(cmd->path, cmd->args[0]);
    if (validate_res != 0)
        return (validate_res);
    if (in_child)
    {
        if (handle_redirections(cmd->redirections) != 0)
            return (1);
        if (fd != STDOUT_FILENO)
            dup2(fd, STDOUT_FILENO);
        exec_child(cmd, env);
    }
    pid = fork();
    if (pid == -1)
    {
        perror("fork");
        return (1);
    }
    if (pid == 0)
    {
        if (handle_redirections(cmd->redirections) != 0)
            exit(1);
        if (fd != STDOUT_FILENO)
            dup2(fd, STDOUT_FILENO);
        exec_child(cmd, env);
    }
    return (wait_child(pid));
}

int exec_cmd_ex(t_command *cmd, t_env **env, int fd, int in_child)
{
    int original_stdin;
    int original_stdout;
    int builtin_result;

    if ((!cmd || !cmd->args || !cmd->args[0]) && !cmd->redirections)
        return (1);
    if (!in_child)
    {
        if (setup_redirections_and_fd(cmd, fd, &original_stdin, &original_stdout) != 0)
            return (1);
    }
    else
    {
        // In child mode, caller already set up stdin/stdout (pipes). Only process redirs here.
        if (handle_redirections(cmd->redirections) != 0)
            return (1);
        if (fd != STDOUT_FILENO)
            dup2(fd, STDOUT_FILENO);
    }
    if (!cmd->args[0])
    {
        if (!in_child)
            restore_file_descriptors(original_stdin, original_stdout);
        return (0);
    }

    if (!in_child)
    {
        builtin_result = handle_builtin_execution(cmd, env, original_stdin, original_stdout);
        if (builtin_result != NOT_BUILTIN)
            return (builtin_result);
        restore_file_descriptors(original_stdin, original_stdout);
    }
    else
    {
        // In child, run builtin directly without saving/restoring parent fds
        t_builtin built_res = exec_builtin(cmd, env, STDOUT_FILENO);
        if (built_res != NOT_BUILTIN)
            return (built_res);
    }

    return handle_external_command_mode(cmd, env, fd, in_child);
}

int exec_cmd(t_command *cmd, t_env **env, int fd)
{
    return exec_cmd_ex(cmd, env, fd, 0);
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
