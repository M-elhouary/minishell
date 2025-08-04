/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_words.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houa <mel-houa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 20:02:58 by mel-houa          #+#    #+#             */
/*   Updated: 2025/08/04 23:56:13 by mel-houa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


char	*remove_quotes(char *str)
{
	char	*new;
	int		i;
	int		j;

	i = 0;
	j = 0;
	if (!str)
		return (NULL);
	new = malloc(ft_strlen(str) + 1);
	if (!new)
		return (NULL);
	while (str[i])
	{
		if (str[i] != '\'' && str[i] != '\"')
			new[j++] = str[i];
		i++;
	}
	new[j] = '\0';
	free(str);
	return (new);
}


static int	skip_quoted_section(const char *line, int *i, char quote)
{
	(*i)++; 
	while (line[*i] && line[*i] != quote)
		(*i)++;
	if (!line[*i])
		return (0); 
	(*i)++; 
	return (1);
}

char	*extract_word(const char *line, int *i)
{
	int		start;
	char	*word;

	start = *i;
	while (line[*i] && !is_metacharacter(line[*i]))
	{
		if (line[*i] == '\'' || line[*i] == '"')
		{
			// skip second quote
			if (!skip_quoted_section(line, i, line[*i]))
				break;
		}
		else
			(*i)++;
	}
	word = ft_strndup(line + start, *i - start);
	return (word);
}