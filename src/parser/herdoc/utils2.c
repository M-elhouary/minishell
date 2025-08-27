/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-abbo <sel-abbo@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 10:17:23 by mel-houa          #+#    #+#             */
/*   Updated: 2025/08/27 07:07:29 by sel-abbo         ###   ########.fr       */
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
			print_error(ctx->delimiter, "warning: here-document wanted> ");
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

	pid = fork();
	if (pid < 0)
		return (-1);
	if (pid != 0)
	{
		random_nb = pid;
		*file_name = gen_file_name_gc(tmp->next->value, random_nb, gc);
		*fd = open(*file_name, O_WRONLY | O_CREAT | O_TRUNC, 0600);
		get_herdoc_fd(*fd);
		if (*fd < 0)
			return (-1);
	}
	if (pid == 0)
	{
		t_clean **cleanchild;
		cleanchild = grepclean();
		cleanup_env((*cleanchild)->env);
		gc_free_all((*cleanchild)->gc);
		free((*cleanchild)->cmd);
		free(*cleanchild);
		exit(0);
	}
	return (0);
}
