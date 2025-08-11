/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   func_help.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houa <mel-houa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 21:13:44 by mel-houa          #+#    #+#             */
/*   Updated: 2025/08/03 23:28:19 by mel-houa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse.h"

char	*get_key(char *env_str)
{
	int		i;
	char	*key;

	i = 0;
	while (env_str[i] && env_str[i] != '=')
		i++;
	key = ft_strndup(env_str, i);
	return (key);
}

char	*get_content(char *env_str)
{
	int		i;
	char	*content;

	i = 0;
	while (env_str[i] && env_str[i] != '=') // increment to name = content
		i++;
	if (!env_str[i])
		return (NULL);
	content = ft_strdup(env_str + i + 1);
	return (content);
}