/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   func_utils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houa <mel-houa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 23:30:02 by mel-houa          #+#    #+#             */
/*   Updated: 2025/08/04 16:48:00 by mel-houa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


char	*ft_strjoin_free(char *s1, char *s2)
{
	char	*result;

	result = ft_strjoin(s1, s2);
	free(s1);
	free(s2);
	return (result);
}



int	skip_spaces(const char *line, int *i)
{
	while (line[*i] && is_space(line[*i]))
		(*i)++;
	return (line[*i] != '\0');
}



int	is_metacharacter(char c)
{
	return (c == '|' || c == '<' || c == '>' || c == ' ' 
		|| c == '\t' || c == '\n');
}

int	is_redirection(t_token_type type)
{
	return (type == REDIR_IN || type == REDIR_OUT
		|| type == REDIR_APPEND || type == HEREDOC);
}

