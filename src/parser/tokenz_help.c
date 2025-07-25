/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenz_help.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houa <mel-houa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 17:15:23 by mel-houa          #+#    #+#             */
/*   Updated: 2025/07/25 03:05:01 by mel-houa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"




// static int	handle_quotes_in_word(const char *line, int *i)
// {
// 	char	quote;

// 	quote = line[*i];
// 	(*i)++;
// 	while (line[*i] && line[*i] != quote)
// 		(*i)++;
// 	if (!line[*i])
// 	{
// 		ft_putstr_fd("minishell: unclosed quote\n", 2);
// 		return (0);
// 	}
// 	(*i)++;
// 	return (1);
// }

// char	*extract_word(const char *line, int *i)
// {
// 	int		start;
// 	char	*word;

// 	start = *i;
// 	while (line[*i] && !is_metacharacter(line[*i]))
// 	{
// 		if (line[*i] == '\'' || line[*i] == '"')
// 		{
// 			if (!handle_quotes_in_word(line, i))
// 				return (NULL);
// 		}
// 		else
// 			(*i)++;
// 	}
// 	word = ft_strndup(line + start, *i - start);
// 	return (word);
// }