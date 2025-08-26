/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: houardi <houardi@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 20:03:12 by mel-houa          #+#    #+#             */
/*   Updated: 2025/08/26 06:30:52 by houardi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse.h"

// add commants to the environment list
char	*get_env_value(char *key, t_env *env)
{
	if (!key || !*key)
		return (ft_strdup("$"));
	while (env)
	{
		if (ft_strcmp(env->key, key) == 0)
			return (ft_strdup(env->content));
		env = env->next;
	}
	return (ft_strdup(""));
}

t_env	*env_new_node(char *key, char *content)
{
	t_env	*new_node;

	new_node = (t_env *)malloc(sizeof(t_env));
	if (!new_node)
		return (NULL);
	new_node->key = key;
	new_node->content = content;
	new_node->exported_only = 0;
	new_node->next = NULL;
	return (new_node);
}

void	env_add_node(t_env **head, t_env *new)
{
	t_env	*tmp;

	if (!head || !new)
		return ;
	if (!*head)
		*head = new;
	else
	{
		tmp = *head;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
	}
}

static void	process_env_entry(char *env_entry, t_env **env_list)
{
	char	*key;
	char	*content;

	key = get_key(env_entry);
	content = get_content(env_entry);
	if (!key || !content)
	{
		free(key);
		free(content);
		return ;
	}
	env_add_node(env_list, env_new_node(key, content));
}

t_env	*my_env(char **env)
{
	t_env	*env_list;
	int		i;

	env_list = NULL;
	i = 0;
	while (env && env[i])
	{
		process_env_entry(env[i], &env_list);
		i++;
	}
	return (env_list);
}
