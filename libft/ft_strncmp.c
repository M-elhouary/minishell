/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houa <mel-houa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 17:55:23 by mel-houa          #+#    #+#             */
/*   Updated: 2024/11/16 20:34:07 by mel-houa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	index;

	index = 0;
	if (!s1[index] && !s2[index])
		return (0);
	while (index < n && (s1[index] || s2[index]))
	{
		if (s1[index] != s2[index])
		{
			return ((unsigned char)s1[index] - (unsigned char)s2[index]);
		}
		index++;
	}
	return (0);
}
