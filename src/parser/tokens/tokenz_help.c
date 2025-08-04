/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenz_help.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houa <mel-houa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 17:15:23 by mel-houa          #+#    #+#             */
/*   Updated: 2025/08/04 16:47:24 by mel-houa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"



char	*remove_syntactic_quotes(char *str)
{
	int		len;
	char	*new;
	
	if (!str)
		return (NULL);
	
	len = ft_strlen(str);
	if (len >= 2)
	{
		// Only remove matching outer quotes
		if ((str[0] == '\'' && str[len-1] == '\'') ||
			(str[0] == '"' && str[len-1] == '"'))
		{
			new = ft_strndup(str+1, len-2);
			free(str);
			return (new);
		}
	}
	return (str);
}


int free_token(t_token *token)
{
	t_token *tmp;

	while(token)
	{
		tmp = token;
		token = token->next;
		if(tmp->value)
			free(tmp->value);
		free(tmp);
	}
	return (1);
}
