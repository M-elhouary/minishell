#include "minishell.h"

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


static t_env	*env_new_node(char *key, char *content)
{
	t_env	*new_node;

	new_node = (t_env *)malloc(sizeof(t_env));
	if (!new_node)
		return (NULL);
	new_node->key = key;
	new_node->content = content;
	new_node->next = NULL;
	return (new_node);
}

static void	env_add_node(t_env **head, t_env *new)
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

static char	*get_key(char *env_str)
{
	int		i;
	char	*key;

	i = 0;
	while (env_str[i] && env_str[i] != '=')
		i++;
	key = ft_strndup(env_str, i);
	return (key);
}

static char	*get_content(char *env_str)
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

t_env	*my_env(char **env)
{
	t_env	*env_list;
	char	*key;
	char	*content;
	int		i;

	env_list = NULL;
	i = 0;
	while (env && env[i])
	{
		key = get_key(env[i]);
		content = get_content(env[i]);
		if (!key || !content)
		{
			free(key);
			free(content);
			i++;
			continue ;
		}
		env_add_node(&env_list, env_new_node(key, content));
		i++;
	}
	return (env_list);
}