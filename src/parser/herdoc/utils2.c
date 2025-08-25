/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houa <mel-houa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 10:17:23 by mel-houa          #+#    #+#             */
/*   Updated: 2025/08/25 06:56:47 by mel-houa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	process_readline_input(char *line, char *clean_delimiter)
{
	if (!line)
		return (0);
	if (ft_strcmp(line, clean_delimiter) == 0)
	{
		free(line);
		return (-1);
	}
	return (1);
}

void	write_raw_line(char *line, int fd)
{
	write(fd, line, ft_strlen(line));
	write(fd, "\n", 1);
	free(line);
}

void	process_herdoc_line(char *line, int fd, t_heredoc_ctx *ctx)
{
	char	*expanded;

	expanded = expand_var_in_string_gc(line, ctx->env_list, ctx->cmd, ctx->gc);
	free(line);
	write(fd, expanded, ft_strlen(expanded));
	write(fd, "\n", 1);
}

void	heredoc_readline_loop(t_heredoc_ctx *ctx, char *clean_delimiter,
		int quotes_for_expansion)
{
	char	*line;
	int		read_result;

	while (1)
	{
		line = readline(">");
		read_result = process_readline_input(line, clean_delimiter);
		if (read_result == 0)
		{
			print_error(ctx->delimiter,
				"warning: here-document wanted> ");
			break ;
		}
		if (read_result == -1)
			break ;
		if (!quotes_for_expansion)
			process_herdoc_line(line, ctx->fd, ctx);
		else
			write_raw_line(line, ctx->fd);
	}
}

int	fork_for_file_name(t_token *tmp, t_gc *gc, char **file_name, int *fd)
{
	static int	random_nb;
	int			pid;

	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	pid = fork();
	if (pid < 0)
		return (-1);
	if (pid != 0)
	{
		random_nb = pid;
		*file_name = gen_file_name_gc(tmp->next->value, random_nb, gc);
		*fd = open(*file_name, O_WRONLY | O_CREAT | O_TRUNC, 0600);
		if (*fd < 0)
			return (-1);
	}
	if (pid == 0)
		exit(0);
	return (0);
}
