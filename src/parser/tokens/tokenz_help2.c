/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenz_help2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houa <mel-houa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 00:13:38 by mel-houa          #+#    #+#             */
/*   Updated: 2025/08/22 11:16:50 by mel-houa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*create_token_gc(char *value, t_token_type type, t_gc *gc)
{
	t_token	*new;

	new = gc_malloc(gc, sizeof(t_token));
	if (!new)
		return (NULL);
	new->value = value;
	new->type = type;
	new->is_empty_expansion = 0;
	new->next = NULL;
	return (new);
}

void	add_token(t_token **head, t_token *new)
{
	t_token	*tmp;

	if (!new)
		return ;
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

t_token	*get_last_token(t_token_ctx *ctx)
{
	t_token	*last;

	last = NULL;
	if (*ctx->tokens)
	{
		last = *ctx->tokens;
		while (last->next)
			last = last->next;
	}
	return (last);
}

int	add_split_words(t_token **tokens, char **split_words, t_gc *gc)
{
	t_token	*new;
	int		j;

	j = 0;
	while (split_words[j])
	{
		if (*tokens == NULL)
			new = create_token_gc(split_words[j], COMMAND, gc);
		else
			new = create_token_gc(split_words[j], ARGUMENT, gc);
		if (!new)
			return (0);
		add_token(tokens, new);
		j++;
	}
	return (1);
}

int	handle_double_char_op(t_token **tokens, char *line, int *i, t_gc *gc)
{
	if (!ft_strncmp(&line[*i], ">>", 2))
	{
		create_and_add_token(tokens, ">>", REDIR_APPEND, gc);
		(*i) += 2;
		return (1);
	}
	else if (!ft_strncmp(&line[*i], "<<", 2))
	{
		create_and_add_token(tokens, "<<", HEREDOC, gc);
		(*i) += 2;
		return (1);
	}
	return (0);
}