/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houa <mel-houa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 12:03:01 by mel-houa          #+#    #+#             */
/*   Updated: 2025/08/27 03:07:35 by mel-houa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*random_char_gc(char *s, int random_nb, t_gc *gc)
{
	char	*itoa_result;
	char	*gc_itoa;
	char	*gc_s;
	char	*result;

	itoa_result = ft_itoa(random_nb);
	gc_itoa = gc_strdup(gc, itoa_result);
	gc_s = gc_strdup(gc, s);
	result = ft_strjoin_free_gc(gc_itoa, gc_s, gc);
	free(itoa_result);
	return (result);
}

int	is_delimiter_quoted(char *token_value)
{
	int	len;

	if (!token_value)
		return (0);
	len = ft_strlen(token_value);
	return ((token_value[0] == '\'' || token_value[0] == '\"')
		&& token_value[len - 1] == token_value[0]);
}

char	*gen_file_name_gc(char *s, int random_nb, t_gc *gc)
{
	char	*name;
	char	*prefix;
	char	*result;

	name = random_char_gc(s, random_nb, gc);
	prefix = gc_strdup(gc, "/tmp/.heredoc_");
	result = ft_strjoin_free_gc(prefix, name, gc);
	return (result);
}

void	free_and_close_gc(char *clean_delimiter, int fd, char *delimiter,
		t_gc *gc)
{
	(void)clean_delimiter;
	(void)delimiter;
	(void)gc;
	close(fd);
}

int	prepare_delimiter_gc(char **clean_delimiter, char *delimiter,
		int *quotes_flag, t_gc *gc)
{
	*quotes_flag = has_quotes(delimiter);
	*clean_delimiter = remove_quotes_gc(gc_strdup(gc, delimiter), gc);
	if (!*clean_delimiter)
		return (0);
	return (1);
}
