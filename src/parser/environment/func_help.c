/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   func_help.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-abbo <sel-abbo@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 21:13:44 by mel-houa          #+#    #+#             */
/*   Updated: 2025/08/27 05:17:08 by sel-abbo         ###   ########.fr       */
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
	while (env_str[i] && env_str[i] != '=')
		i++;
	if (!env_str[i])
		return (NULL);
	content = ft_strdup(env_str + i + 1);
	return (content);
}
