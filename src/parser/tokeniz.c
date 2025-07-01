/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokeniz.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houa <mel-houa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 14:57:18 by mel-houa          #+#    #+#             */
/*   Updated: 2025/07/01 14:52:48 by mel-houa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*new_tok(char *value, t_token_type type)
{
	t_token	*new_token;

	new_token = (t_token *)malloc(sizeof(t_token));
	if (!new_token)
		return (NULL);
	new_token->type = type;
	new_token->value = value;
	new_token->next = NULL;
	return (new_token);
}
void	add_tok(t_token **head, t_token *new)
{
	t_token	*tmp;

	if (!*head)
		*head = new;
	else
	{
		tmp = *head;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
	}
}

char *extract_word(const char *line, int *i)
{
    int start = *i;
    int len = 0;
    char quote = 0;
    char *word = NULL;


    while (line[*i] && !is_space(line[*i]) && line[*i] != '>' && line[*i] != '<' && line[*i] != '|')
    {
        if (line[*i] == '\'' || line[*i] == '"')
        {
            quote = line[(*i)++];
            while (line[*i] && line[*i] != quote)
                (*i)++;
            if (line[*i] == quote)
                (*i)++;
            else
                return (NULL); // Unclosed quote
        }
        else
            (*i)++;
    }
    len = *i - start;
    word = (char *)malloc(len + 1);
    if (!word)
        return (NULL);
    strncpy(word, line + start, len);
    word[len] = '\0';
    return (word);
}


t_token	*tokenze(char *line)
{
	int		i;
	char	*word;
	t_token	*tokinzes;
	t_token	*quote_token;

	tokinzes = NULL;
	quote_token = NULL;
	word = NULL;
	i = 0;
	while (line[i])
	{
		if (line[i] == '\'' || line[i] == '"')
		{
			i++;
			while (is_space(line[i]))
				i++;
			printf("%d\n", i);
			quote_token = handl_quote(line, &i);
			if (!quote_token)
				return (NULL);
			add_tok(&tokinzes, quote_token);
		}
		else
		{
			if (is_space(line[i]))
				i++;
			else if (line[i] == '|')
			{
				add_tok(&tokinzes, new_tok(ft_strdup("|"), PIPE));
				i++;
			}
			else if (line[i] == '>' && line[i + 1] == '>')
			{
				add_tok(&tokinzes, new_tok(ft_strdup(">>"), REDIR_APPEND));
				i += 2;
			}
			else if (line[i] == '<' && line[i + 1] == '<')
			{
				add_tok(&tokinzes, new_tok(ft_strdup("<<"), HEREDOC));
				i += 2;
			}
			else if (line[i] == '<')
			{
				add_tok(&tokinzes, new_tok(ft_strdup("<"), REDIR_IN));
				i++;
			}
			else if (line[i] == '>')
			{
				add_tok(&tokinzes, new_tok(ft_strdup(">"), REDIR_OUT));
				i++;
			}
			else
			{
                    word = extract_word(line, &i);
                    if (!word)
                    {
                        free_token(tokinzes); // implement this to free your token list
                        return (NULL);
                    }
				    add_tok(&tokinzes, new_tok(word, WORD));
			}
		}
	}
	if (!check_syntax_token(tokinzes))
		return (NULL);
	return (tokinzes);
}
