/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   herdoc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houa <mel-houa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 15:18:28 by mel-houa          #+#    #+#             */
/*   Updated: 2025/08/21 07:01:42 by mel-houa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


// struc
typedef struct s_heredoc_ctx
{
	char		*delimiter;
	int			fd;
	t_env		*env_list;
	t_command	*cmd;
	t_gc		*gc;
}	t_heredoc_ctx;

static int	prepare_heredoc_delimiter(t_heredoc_ctx *ctx, char **clean_delimiter)
{
	int	quotes_for_expansion;

	// Always use non-GC version in child process for safety
	if (!prepare_delimiter(clean_delimiter, ctx->delimiter, 
			&quotes_for_expansion))
	{
		close(ctx->fd);
		free(ctx->delimiter);
		return (0);
	}
	return (quotes_for_expansion);
}

static void	process_heredoc_line(char *line, t_heredoc_ctx *ctx, int quotes_flag)
{
	char	*expanded;

	if (!quotes_flag)
	{
		expanded = expand_var_in_string(line, ctx->env_list, ctx->cmd);
		free(line);
		write(ctx->fd, expanded, ft_strlen(expanded));
		write(ctx->fd, "\n", 1);
		free(expanded);
	}
	else
	{
		write(ctx->fd, line, ft_strlen(line));
		write(ctx->fd, "\n", 1);
		free(line);
	}
}

void	similation_herdoc_gc(char *delimiter, int fd, t_env *env_list, 
		t_command *cmd)
{
	t_heredoc_ctx	ctx;
	char			*line;
	char			*clean_delimiter;
	int				quotes_flag;

	ctx = (t_heredoc_ctx){delimiter, fd, env_list, cmd, NULL};
	signal(SIGINT, sigint_child_handler);
	quotes_flag = prepare_heredoc_delimiter(&ctx, &clean_delimiter);
	if (quotes_flag == 0 && !clean_delimiter)
	{
		free(delimiter);
		close(fd);
		exit(1);
	}
	while (1)
	{
		line = readline(">");
		if (!line)
		{
			print_error(delimiter, "warning: here-document at line 1 delimited by end-of-file wanted ==> ");
			break ;
		}
		if (ft_strcmp(line, clean_delimiter) == 0)
		{
			free(line);
			break ;
		}
		process_heredoc_line(line, &ctx, quotes_flag);
	}
	free(clean_delimiter);
	free(delimiter);
	close(fd);
}

static int	process_heredoc_token_gc(t_token *tmp, t_env *env_list, t_command *cmd, t_gc *gc)
{
	char		*file_name;
	int			pid_for_name;
	int			pid;
	int			wait_result;
	int			status;
	int			fd;

	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	// First fork to get PID for file name generation
	pid_for_name = fork();
	if (pid_for_name < 0)
		return (0);
	if (pid_for_name == 0)
		exit(0);
	// Wait for first child and generate file name
	waitpid(pid_for_name, NULL, 0);//>>
	file_name = gen_file_name_gc(tmp->value, pid_for_name, gc);
	fd = open(file_name, O_WRONLY | O_CREAT | O_TRUNC, 0600);
	if (fd < 0)
		return (0);
	// Second fork for heredoc processing
	pid = fork();
	if (pid == 0)
	{
		similation_herdoc_gc(strdup(tmp->next->value), fd, env_list, cmd);
		exit(0);
	}
	close(fd);
	wait_result = waitpid(pid, &status, 0);
	if (wait_result == -1)
		return (0);
	if (WIFEXITED(status))
	{
		cmd->status_exit = WEXITSTATUS(status);
		if (cmd->status_exit == 130)  // Interrupted by SIGINT
			return (130);
		tmp->next->value = file_name;
		return (cmd->status_exit);
	}
	if (WIFSIGNALED(status))
	{
		cmd->status_exit = 128 + WTERMSIG(status);
		return (cmd->status_exit);
	}
	return (0);
}

int	handl_herdoc_gc(t_token *token, t_env *env_list, t_command *cmd, t_gc *gc)
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
			if (process_heredoc_token_gc(tmp, env_list, cmd, gc))
				return (1) ; // Stop if heredoc processing failed or was interrupted
		}
		if (tmp)
			tmp = tmp->next;
	}
	return 0;
}
