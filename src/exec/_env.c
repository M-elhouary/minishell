/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   _env.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hayabusa <hayabusa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 03:51:57 by houardi           #+#    #+#             */
/*   Updated: 2025/08/25 13:29:16 by hayabusa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	set_env_value(t_env **env, char *key, char *value)
{
	t_env	*current;
	char	*new_key;
	char	*new_value;

	current = *env;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			free(current->content);
			current->content = ft_strdup(value);
			current->exported_only = 0;
			return (!current->content);
		}
		current = current->next;
	}
	new_key = ft_strdup(key);
	new_value = ft_strdup(value);
	if (!new_key || !new_value)
	{
		free(new_key);
		free(new_value);
		return (1);
	}
	env_add_node(env, env_new_node(new_key, new_value));
	return (0);
}

int	set_env_exported_only(t_env **env, char *key)
{
	t_env	*current;
	char	*new_key;

	current = *env;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			current->exported_only = 1;
			if (current->content)
			{
				free(current->content);
				current->content = NULL;
			}
			return (0);
		}
		current = current->next;
	}
	new_key = ft_strdup(key);
	if (!new_key)
		return (1);
	current = env_new_node(new_key, NULL);
	if (!current)
	{
		free(new_key);
		return (1);
	}
	current->exported_only = 1;
	env_add_node(env, current);
	return (0);
}

int	unset_env_value(t_env **env, char *key)
{
	t_env *current = *env;
	t_env *prev = NULL;

	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			if (prev)
				prev->next = current->next;
			else
				*env = current->next;
			free(current->key);
			free(current->content);
			free(current);
			return (0);
		}
		prev = current;
		current = current->next;
	}
	return (1);
}

char	**env_to_array(t_env *env)
{
	t_env *current = env;
	char **envp;
	int count = 0;
	int i = 0;
	int key_len, content_len;
	
	// Count only variables that should be in the environment
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
		// Skip exported_only variables and variables with NULL content
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

void	free_env_array(char **envp)
{
	int i = 0;
	
	if (!envp)
		return;
	while (envp[i])
	{
		free(envp[i]);
		i++;
	}
	free(envp);
}
