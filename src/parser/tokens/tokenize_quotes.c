/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_quotes.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houa <mel-houa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 20:02:53 by mel-houa          #+#    #+#             */
/*   Updated: 2025/08/21 18:49:35 by mel-houa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_quotes(const char *line, int *i)
{
	char	quote;

	if (line[*i] != '\'' && line[*i] != '\"')
		return (0);
	quote = line[*i];
	(*i)++;
	while (line[*i] && line[*i] != quote)
	{
		if (line[*i] == '\\' && quote == '\"')
			(*i)++;
		if (line[*i])
			(*i)++;
	}
	if (!line[*i])
		return (print_error("unclosed quote", NULL), 0);
	(*i)++;
	return (1);
}

char	*extract_quoted(const char *line, int *i)
{
	char quote;
	int start;
	int len;

	quote = line[*i];
	start = *i;
	(*i)++;
	len = 0;
	while (line[*i] && line[*i] != quote)
	{
		(*i)++;
		len++;
	}
	if (!line[*i])
	{
		return (ft_substr(line, start, *i - start));
	}
	(*i)++;
	return (ft_substr(line, start + 1, len));
}