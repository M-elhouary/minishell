/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   garbage_collector.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houa <mel-houa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 20:03:25 by mel-houa          #+#    #+#             */
/*   Updated: 2025/08/21 16:52:16 by mel-houa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"

void	gc_init(t_gc *gc)
{
	if (!gc)
		return ;
	gc->head = NULL;
}

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

void	gc_free_all(t_gc *gc)
{
	t_gc_node	*current;
	t_gc_node	*next;

	if (!gc)
		return ;
	current = gc->head;
	while (current)
	{
		next = current->next;
		free(current->ptr);
		free(current);
		current = next;
	}
	gc->head = NULL;
}

void	gc_destroy(t_gc *gc)
{
	if (!gc)
		return ;
	gc_free_all(gc);
}

char	*gc_strndup(t_gc *gc, const char *s, size_t n)
{
	char	*dup;
	size_t	i;

	if (!s || !gc)
		return (NULL);
	dup = gc_malloc(gc, n + 1);
	if (!dup)
		return (NULL);
	i = 0;
	while (i < n && s[i])
	{
		dup[i] = s[i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
}

static void	copy_strings_gc(char *result, char *s1, char *s2)
{
	int	i;
	int	j;

	i = 0;
	while (s1[i])
	{
		result[i] = s1[i];
		i++;
	}
	j = 0;
	while (s2[j])
		result[i++] = s2[j++];
	result[i] = '\0';
}

char	*ft_strjoin_free_gc(char *s1, char *s2, t_gc *gc)
{
	char	*result;
	int		len1;
	int		len2;

	if (!s1 || !s2)
		return (NULL);
	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	result = gc_malloc(gc, len1 + len2 + 1);
	if (!result)
		return (NULL);
	copy_strings_gc(result, s1, s2);
	return (result);
}
