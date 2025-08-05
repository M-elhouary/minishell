/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_words.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houa <mel-houa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 20:02:58 by mel-houa          #+#    #+#             */
/*   Updated: 2025/08/05 03:40:52 by mel-houa         ###   ########.fr       */
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


// static int	skip_quoted_section(const char *line, int *i, char quote)
// {
// 	(*i)++; 
// 	while (line[*i] && line[*i] != quote)
// 		(*i)++;
// 	if (!line[*i])
// 		return (0); 
// 	(*i)++; 
// 	return (1);
// }

char *extract_word(const char *line, int *i)
{
    // int start = *i;
    int j = 0;
    char *word = malloc(ft_strlen(line) + 1);
    if (!word)
        return NULL;

    while (line[*i] && !is_space(line[*i]) && !is_metacharacter(line[*i]))
    {
        if (line[*i] == '\'' || line[*i] == '"')
        {
            char quote = line[(*i)++];
            word[j++] = quote; // Optionally keep the quote, or skip if you want to remove
            while (line[*i] && line[*i] != quote)
                word[j++] = line[(*i)++];
            if (line[*i] == quote)
                word[j++] = line[(*i)++]; // Optionally keep the quote, or skip
        }
        else
        {
            word[j++] = line[(*i)++];
        }
    }
    word[j] = '\0';
    return word;
}