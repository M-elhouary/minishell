/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   herdoc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houa <mel-houa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 03:46:58 by mel-houa          #+#    #+#             */
/*   Updated: 2025/08/09 03:50:07 by mel-houa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"

void expand_heredoc(t_token *token)
{
    char *expanded;
    t_token *tmp;

    if (!token || token->type != HEREDOC)
        return;

    tmp = token;
    while (tmp)
    {
        if (tmp->type == HEREDOC)
        {
            // Expand the heredoc token
            free(tmp->value);
            tmp->value = expanded;
        }
        tmp = tmp->next;
    }
	// Implement heredoc expansion logic here
}


void	handle_heredoc(t_token *tokens)
{
	t_token	*tmp;

	tmp = tokens;
	while (tmp)
	{
		if (tmp->type == HEREDOC)
		{
			// Handle heredoc expansion
			expand_heredoc(tmp);
		}
		tmp = tmp->next;
	}
}