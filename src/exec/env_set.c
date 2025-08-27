/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_set.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: houardi <houardi@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 03:51:57 by houardi           #+#    #+#             */
/*   Updated: 2025/08/26 09:07:03 by houardi          ###   ########.fr       */
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
	return (env_add_node(env, env_new_node(new_key, new_value)), 0);
}

int add_new_key(char *new_key, t_env **current)
{
	if (!(new_key))
		return (1);
	*current = env_new_node(new_key, NULL);
	if (!(*current))
	{
		free(new_key);
		return (1);
	}
	(*current)->exported_only = 1;
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
	if (add_new_key(new_key, &current) == 1)
		return (1);
	return (env_add_node(env, current), 0);
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
