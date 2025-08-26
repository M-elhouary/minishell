/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_help_func.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houa <mel-houa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 19:31:20 by mel-houa          #+#    #+#             */
/*   Updated: 2025/08/22 10:51:57 by mel-houa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	gc_init(t_gc *gc)
{
	if (!gc)
		return ;
	gc->head = NULL;
}

void	copy_strings_gc(char *result, char *s1, char *s2)
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
