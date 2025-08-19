/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houa <mel-houa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 12:03:01 by mel-houa          #+#    #+#             */
/*   Updated: 2025/08/16 21:48:48 by mel-houa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*random_char(int index, char *s)
{
	char	*tmp;
	char	*charset;

	charset = ft_strdup("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789");
	tmp = ft_strjoin(charset + index, s);
	free(charset);
	return (tmp);
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

char	*gen_file_name(int index, char *s)
{
	char	*name;
	char	*tmp;

	name = random_char(index, s);
	tmp = ft_strjoin("/tmp/heredoc_", name);
	free(name);
	return (tmp);
}

/* Helper function to free resources and close fd */
void	free_and_close(char *clean_delimiter, int fd, char *delimiter)
{
	free(clean_delimiter);
	close(fd);
	free(delimiter);
}


int	prepare_delimiter(char **clean_delimiter, char *delimiter, int *quotes_flag)
{
	*quotes_flag = has_quotes(delimiter);
	*clean_delimiter = remove_quotes(ft_strdup(delimiter));
	if (!*clean_delimiter)
		return (0);
	return (1);
}