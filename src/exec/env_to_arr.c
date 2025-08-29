/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_to_arr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: houardi <houardi@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 08:17:06 by houardi           #+#    #+#             */
/*   Updated: 2025/08/27 06:13:08 by houardi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_valid_env_vars(t_env *env)
{
	t_env	*current;
	int		count;

	current = env;
	count = 0;
	while (current)
	{
		if (!current->exported_only && current->content != NULL)
			count++;
		current = current->next;
	}
	return (count);
}

char	*create_env_string(t_env *env_node)
{
	char	*env_str;
	int		key_len;
	int		content_len;

	key_len = ft_strlen(env_node->key);
	content_len = ft_strlen(env_node->content);
	env_str = malloc(key_len + content_len + 2);
	if (!env_str)
		return (NULL);
	ft_strcpy(env_str, env_node->key);
	ft_strcat(env_str, "=");
	ft_strcat(env_str, env_node->content);
	return (env_str);
}

void	free_env_array_partial(char **envp, int count)
{
	while (--count >= 0)
		free(envp[count]);
	free(envp);
}

int	env_array(t_env *env, char **envp, int max_count)
{
	t_env	*current;
	int		i;

	current = env;
	i = 0;
	while (current && i < max_count)
	{
		if (current->exported_only || current->content == NULL)
		{
			current = current->next;
			continue ;
		}
		envp[i] = create_env_string(current);
		if (!envp[i])
		{
			free_env_array_partial(envp, i);
			return (-1);
		}
		current = current->next;
		i++;
	}
	return (i);
}

char	**env_to_array(t_env *env)
{
	char	**envp;
	int		count;
	int		result;

	count = count_valid_env_vars(env);
	envp = malloc(sizeof(char *) * (count + 1));
	if (!envp)
		return (NULL);
	result = env_array(env, envp, count);
	if (result == -1)
		return (NULL);
	envp[count] = NULL;
	return (envp);
}
