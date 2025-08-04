/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_quotes.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houa <mel-houa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 20:02:53 by mel-houa          #+#    #+#             */
/*   Updated: 2025/08/03 23:28:20 by mel-houa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse.h"

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