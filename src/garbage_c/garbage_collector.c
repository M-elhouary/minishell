/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   garbage_collector.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houa <mel-houa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 20:03:25 by mel-houa          #+#    #+#             */
/*   Updated: 2025/08/21 20:24:13 by mel-houa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"



void	*gc_malloc(t_gc *gc, size_t size)
{
	void		*ptr;
	t_gc_node	*new_node;

	if (!gc)
		return (NULL);
	ptr = malloc(size);
	if (!ptr)
		return (NULL);
	new_node = malloc(sizeof(t_gc_node));
	if (!new_node)
	{
		free(ptr);
		return (NULL);
	}
	new_node->ptr = ptr;
	new_node->next = gc->head;
	gc->head = new_node;
	return (ptr);
}

char	*gc_strdup(t_gc *gc, const char *s)
{
	char	*dup;
	size_t	len;

	if (!s || !gc)
		return (NULL);
	len = ft_strlen(s);
	dup = gc_malloc(gc, len + 1);
	if (!dup)
		return (NULL);
	ft_strcpy(dup, s);
	return (dup);
}

static void	free_temp_split(char **temp_split)
{
	int	i;

	i = 0;
	while (temp_split[i])
		free(temp_split[i++]);
	free(temp_split);
}

static char	**copy_to_gc(t_gc *gc, char **temp_split)
{
	char	**result;
	int		i;

	i = 0;
	while (temp_split[i])
		i++;
	result = gc_malloc(gc, sizeof(char *) * (i + 1));
	if (!result)
		return (NULL);
	i = 0;
	while (temp_split[i])
	{
		result[i] = gc_strdup(gc, temp_split[i]);
		free(temp_split[i]);
		i++;
	}
	result[i] = NULL;
	return (result);
}

char	**gc_split(t_gc *gc, char const *s, char c)
{
	char	**temp_split;
	char	**result;

	if (!s || !gc)
		return (NULL);
	temp_split = ft_split(s, c);
	if (!temp_split)
		return (NULL);
	result = copy_to_gc(gc, temp_split);
	if (!result)
		free_temp_split(temp_split);
	else
		free(temp_split);
	return (result);
}
