/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_checker.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houa <mel-houa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 16:11:58 by mel-houa          #+#    #+#             */
/*   Updated: 2025/08/03 23:28:23 by mel-houa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include "parse.h"


static int	check_pipe_start(t_token *tokens)
{
	if (tokens && tokens->type == PIPE)
	{
		print_error("syntax error near unexpected token `|'", NULL);
		return (0);
	}
	return (1);
}

static int	check_consecutive_pipes(t_token *tokens)
{
	t_token	*tmp;

	tmp = tokens;
	while (tmp && tmp->next)
	{
		if (tmp->type == PIPE && tmp->next->type == PIPE)
		{
			print_error("syntax error near unexpected token `|'", NULL);
			return (0);
		}
		tmp = tmp->next;
	}
	return (1);
}

static int	check_pipe_end(t_token *tokens)
{
	t_token	*tmp;

	tmp = tokens;
	while (tmp && tmp->next)
		tmp = tmp->next;
	if (tmp && tmp->type == PIPE)
	{
		print_error("syntax error near unexpected token `|'", NULL);
		return (0);
	}
	return (1);
}

static int	check_redirection(t_token *tokens)
{
	t_token	*tmp;

	tmp = tokens;
	while (tmp)
	{
		if (is_redirection(tmp->type))
		{
			if (!tmp->next)
			{
				print_error("syntax error near unexpected token `newline'", NULL);
				return (0);
			}
			if (is_redirection(tmp->next->type))
			{
				print_error("syntax error near unexpected token", tmp->next->value);
				return (0);
			}
		}
		tmp = tmp->next;
	}
	return (1);
}

int	check_syntax_token(t_token *tokens)
{
	if (!check_pipe_start(tokens))
		return (0);
	if (!check_consecutive_pipes(tokens))
		return (0);
	if (!check_pipe_end(tokens))
		return (0);
	if (!check_redirection(tokens))
		return (0);
	return (1);
}