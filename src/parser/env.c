#include "minishell.h"

char *get_env_value(char *key, t_env *env)
{
    char *key_not_found;
    while (env)
    {
        if (ft_strcmp(env->key, key) == 0)
            return env->content;
        env = env->next;
    }
    key_not_found = malloc(ft_strlen(key) + 2);
    if (!key_not_found)
        return NULL;
    key_not_found[0] = '$';
    ft_strcpy(key_not_found + 1, key);
    return key_not_found;
}


t_env	*new_node(char *key, char *content)
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


void	add_node(t_env **head, t_env *new)
{
	t_env	*tmp;

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


t_env *my_env(char **env)
{
    int index;
    t_env *variables;
    char *var;
    char *key;
    char *content;
    int i;
    
    index = 0;
    i = 0;
    var = NULL;
    variables = NULL;
    key = NULL;
    content = NULL;
    while(env[index])
    {
        var = ft_strdup(env[index]);
        i = 0;
        while(var[i] != '=')
            i++;
        key = ft_strndup(var, i);
        content = ft_strdup(var+i+1);
        add_node(&(variables), new_node(key, content));
        index++;
    }
    return (variables);
}