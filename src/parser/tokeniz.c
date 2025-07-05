/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokeniz.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houa <mel-houa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 14:57:18 by mel-houa          #+#    #+#             */
/*   Updated: 2025/07/05 23:53:20 by mel-houa         ###   ########.fr       */
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




t_token	*tokenze(char *line, char **env)
{
	int		i;
	char	*word;
	t_token	*tokinzes;
	t_token	*quote_token;
	t_token *tmp;
	char *value;

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
                        free_token(tokinzes);
                        return (NULL);
                    }
				    add_tok(&tokinzes, new_tok(word, WORD));
			}
		}
	}
	if (!check_syntax_token(tokinzes))
		return (NULL);
	tmp = tokinzes;
	while(tmp)
	{
		if(tmp->type == WORD && ft_strchr(tmp->value, '$'))
		{
			
			value = expand_variables(tmp->value, env);
			if(value)
			{
				
				free(tmp->value);
				tmp->value = ft_strdup(value);
			}
			else
			{
				free(tmp->value);
				tmp->value = ft_strdup("");
			}
		}
		tmp = tmp->next;
		
	}
	return (tokinzes);
}
