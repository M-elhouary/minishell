#include "minishell.h"

t_export	*create_export_node(char *var)
{
	t_export	*new_node;

	new_node = malloc(sizeof(t_export));
	if (!new_node)
		return (NULL);
	new_node->var = ft_strdup(var);
	if (!new_node->var)
	{
		free(new_node);
		return (NULL);
	}
	new_node->next = NULL;
	return (new_node);
}

void	add_export_node(t_export **head, t_export *new_node)
{
	t_export	*current;

	if (!head || !new_node)
		return ;
	if (!*head)
	{
		*head = new_node;
		return ;
	}
	current = *head;
	while (current->next)
		current = current->next;
	current->next = new_node;
}

t_export	*find_export_node(t_export *head, char *var)
{
	t_export	*current;

	current = head;
	while (current)
	{
		if (ft_strncmp(current->var, var, ft_strlen(var)) == 0)
			return (current);
		current = current->next;
	}
	return (NULL);
}

void	remove_export_node(t_export **head, char *var)
{
	t_export	*current;
	t_export	*prev;

	if (!head || !*head || !var)
		return ;
	current = *head;
	prev = NULL;
	while (current)
	{
		if (ft_strncmp(current->var, var, ft_strlen(var)) == 0)
		{
			if (prev)
				prev->next = current->next;
			else
				*head = current->next;
			free(current->var);
			free(current);
			return ;
		}
		prev = current;
		current = current->next;
	}
}

void	free_export_list(t_export *head)
{
	t_export	*current;
	t_export	*next;

	current = head;
	while (current)
	{
		next = current->next;
		free(current->var);
		free(current);
		current = next;
	}
}

int	export_list_size(t_export *head)
{
	t_export	*current;
	int			count;

	current = head;
	count = 0;
	while (current)
	{
		count++;
		current = current->next;
	}
	return (count);
}

int	export_empty_var(t_export **export_list, char *var)
{
	t_export	*existing_node;
	t_export	*new_node;

	if (!export_list || !var)
		return (1);
	existing_node = find_export_node(*export_list, var);
	if (existing_node)
		return (0);
	new_node = create_export_node(var);
	if (!new_node)
		return (1);
	add_export_node(export_list, new_node);
	return (0);
}

