/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_words.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houa <mel-houa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 20:02:58 by mel-houa          #+#    #+#             */
/*   Updated: 2025/08/20 23:48:25 by mel-houa         ###   ########.fr       */
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

char	*remove_quotes_gc(char *str, t_gc *gc)
{
	char	*new;
	int		i;
	int		j;

	i = 0;
	j = 0;
	if (!str)
		return (NULL);
	new = gc_malloc(gc, ft_strlen(str) + 1);
	if (!new)
		return (NULL);
	while (str[i])
	{
		if (str[i] != '\'' && str[i] != '\"')
			new[j++] = str[i];
		i++;
	}
	new[j] = '\0';
	return (new);
}


char	*extract_word_gc(char *line, int *i, t_gc *gc)
{
    int		j = 0;
    char	*word;
    char	quote = 0;
    int		word_len = 0;
    
    // Calculate word length (including quotes and adjacent parts)
    while (line[*i + word_len] && (!is_space(line[*i + word_len]) || quote))
    {
        // Track quote state
        if (!quote && (line[*i + word_len] == '\'' || line[*i + word_len] == '"'))
            quote = line[*i + word_len];
        else if (quote && line[*i + word_len] == quote)
            quote = 0;
            
        // Only stop at special characters if not in quotes
        if (!quote && is_metacharacter(line[*i + word_len]) && 
            word_len > 0)  // Don't stop if at beginning of word
            break;
            
        word_len++;
    }
    
    // Allocate and copy the word
    word = gc_malloc(gc, word_len + 1);
    if (!word)
        return (NULL);
        
    j = 0;
    while (j < word_len)
    {
        word[j] = line[*i];
        (*i)++;
        j++;
    }
    word[j] = '\0';
    
    return word;
}