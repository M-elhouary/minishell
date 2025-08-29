/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houa <mel-houa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 15:00:38 by mel-houa          #+#    #+#             */
/*   Updated: 2024/11/16 20:34:19 by mel-houa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	int	pos;

	pos = ft_strlen(s) - 1;
	if (c == '\0')
		return ((char *)(s + ft_strlen(s)));
	while (pos >= 0)
	{
		if (*(s + pos) == (char)c)
		{
			return ((char *)(s + pos));
		}
		pos--;
	}
	return (NULL);
}
