/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   herdoc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houa <mel-houa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 15:18:28 by mel-houa          #+#    #+#             */
/*   Updated: 2025/08/19 02:24:47 by mel-houa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// if del on qotes not expanded

static void	process_herdoc_line(char *line, int fd, t_env *env_list,
		t_command *cmd)
{
	char	*expanded;

	expanded = expand_var_in_string(line, env_list, cmd);
	free(line);
	write(fd, expanded, ft_strlen(expanded));
	write(fd, "\n", 1);
	free(expanded);
}

void	similation_herdoc(char *delimiter, int fd, t_env *env_list,
		t_command *cmd)
{
    int quotes_for_expansion;
    char *line, *clean_delimiter;
	static int random_nb;
    
    /* Save current execution state */
    int old_exec_state = get_execution_state();
    
    if (!prepare_delimiter(&clean_delimiter, delimiter, &quotes_for_expansion))
    {
        close(fd);
        free(delimiter);
        return;
    }
    /* Setup special signal handling for heredoc */
    signal(SIGINT, SIG_DFL);   /* Default SIGINT for heredoc */
    signal(SIGQUIT, SIG_IGN);  /* Ignore SIGQUIT in heredoc */
    while (1)
    {
		random_nb =  12;
        line = readline(">");
        if (!line || ft_strcmp(line, clean_delimiter) == 0)
        {
            free(line);
            break ;
        }
        if (!quotes_for_expansion)
            process_herdoc_line(line, fd, env_list, cmd);
        else
        {
            write(fd, line, ft_strlen(line));
            write(fd, "\n", 1);
            free(line);
        }
		random_nb++;
    }
    /* Restore previous signal handlers */
    set_execution_state(old_exec_state);
    if (get_execution_state() == 0)
    {
        signal(SIGINT, sigint_handler);
        signal(SIGQUIT, sigquit_handler);
    }
    
    free_and_close(clean_delimiter, fd, delimiter);
}

/* Process a heredoc token */
static int	process_heredoc_token(t_token *tmp, t_env *env_list, t_command *cmd)
{
	char	*file_name;
	static int random_nb;
	int	pid;

	int wait_result, status, fd;
	pid = fork();
	if (pid < 0)
		return (0);
	if (pid != 0)
	{
		random_nb = pid;
		file_name = gen_file_name(tmp->value, random_nb);
	}
	fd = open(file_name, O_WRONLY | O_CREAT | O_TRUNC, 0600);
	if (fd < 0)
		return (free(file_name), 0);
	if (pid == 0)
	{
		similation_herdoc(ft_strdup(tmp->next->value), fd, env_list, cmd);
		exit(0);
	}
	close(fd); // Close fd in parent process immediately
	wait_result = waitpid(pid, &status, 0);
	//this mean failed to wait for child process
	if (wait_result == -1)
		return (free(file_name), 0);
	//this mean child process exited successfully
	if (WIFEXITED(status) && WEXITSTATUS(status) == 0)
		return (tmp->next->value = ft_strdup(file_name), free(file_name), 1);
	// this mean child process was interrupted or failed
	else if (WIFSIGNALED(status))
		return (free(file_name), 0);
		
	return (free(file_name), 0);
}

void	handl_herdoc(t_token *token, t_env *env_list, t_command *cmd)
{
	t_token	*tmp;

	tmp = token;
	if (!tmp)
		return ;
	while (tmp)
	{
		if (tmp->type == HEREDOC && tmp->next && tmp->next->type == ARGUMENT
			&& !tmp->next->is_empty_expansion)
		{
			if (!process_heredoc_token(tmp, env_list, cmd))
				return ; // Stop if heredoc processing failed or was interrupted
		}
		if (tmp)
			tmp = tmp->next;
	}
}
