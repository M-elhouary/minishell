/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   herdoc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houa <mel-houa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 15:18:28 by mel-houa          #+#    #+#             */
/*   Updated: 2025/08/19 05:55:45 by mel-houa         ###   ########.fr       */
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
	signal(SIGINT, sigint_child_handler);
    if (!prepare_delimiter(&clean_delimiter, delimiter, &quotes_for_expansion))
    {
        close(fd);
        free(delimiter);
        return;
    }
    while (1)
    {
        line = readline(">");
		if(!line)
		{
			free(line);
			print_error(delimiter, "warning: here-document at line 1 delimited by end-of-file wanted ==> ");
            break ;
		}
        if (ft_strcmp(line, clean_delimiter) == 0)
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
    }
    free_and_close(clean_delimiter, fd, delimiter);
}

/* Process a heredoc token */
static int	process_heredoc_token(t_token *tmp, t_env *env_list, t_command *cmd)
{
	char	*file_name;
	static int random_nb;
	int	pid = 0;

	int wait_result, status, fd;
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	if (pid != 0)
	if (pid < 0)
	return (0);
	pid = fork();
	{
		random_nb = pid;
		file_name = gen_file_name(tmp->value, random_nb);
		fd = open(file_name, O_WRONLY | O_CREAT | O_TRUNC, 0600);
		if (fd < 0)
			return (free(file_name), 0);
	}
	if (pid == 0)
		exit (0);
	pid = fork();
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
	if (WIFEXITED(status))
	{
		cmd->status_exit = WEXITSTATUS(status);
		return (tmp->next->value = ft_strdup(file_name), free(file_name), cmd->status_exit);
	}
	return (free(file_name), 0);
}

int	handl_herdoc(t_token *token, t_env *env_list, t_command *cmd)
{
	t_token	*tmp;

	tmp = token;
	if (!tmp)
		return 1;
	while (tmp)
	{
		if (tmp->type == HEREDOC && tmp->next && tmp->next->type == ARGUMENT
			&& !tmp->next->is_empty_expansion)
		{
			if (process_heredoc_token(tmp, env_list, cmd))
				return (1) ; // Stop if heredoc processing failed or was interrupted
		}
		if (tmp)
			tmp = tmp->next;
	}
	return 0;
}
