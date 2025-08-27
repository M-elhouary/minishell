/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_to_arr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: houardi <houardi@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 08:17:06 by houardi           #+#    #+#             */
/*   Updated: 2025/08/26 08:46:50 by houardi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**env_to_array(t_env *env)
{
	t_env *current = env;
	char **envp;
	int count = 0;
	int i = 0;
	int key_len, content_len;
	
	while (current)
	{
		if (!current->exported_only && current->content != NULL)
			count++;
		current = current->next;
	}
	envp = malloc(sizeof(char*) * (count + 1));
	if (!envp)
		return (NULL);
	current = env;
	while (current && i < count)
	{
		if (current->exported_only || current->content == NULL)
		{
			current = current->next;
			continue;
		}
		key_len = ft_strlen(current->key);
		content_len = ft_strlen(current->content);
		envp[i] = malloc(key_len + content_len + 2);
		if (!envp[i])
		{
			while (--i >= 0)
				free(envp[i]);
			free(envp);
			return (NULL);
		}
		ft_strcpy(envp[i], current->key);
		ft_strcat(envp[i], "=");
		ft_strcat(envp[i], current->content);
		current = current->next;
		i++;
	}
	envp[count] = NULL;
	return (envp);
}
