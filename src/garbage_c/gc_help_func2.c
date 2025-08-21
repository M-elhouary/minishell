/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_help_2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houa <mel-houa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 20:27:04 by mel-houa          #+#    #+#             */
/*   Updated: 2025/08/21 20:28:58 by mel-houa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */




#include "minishell.h"


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