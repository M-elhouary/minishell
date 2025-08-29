/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houa <mel-houa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 21:39:06 by mel-houa          #+#    #+#             */
/*   Updated: 2024/11/16 20:33:16 by mel-houa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	size_t			pos;
	unsigned char	*ptr1dest;
	unsigned char	*ptr2src;

	ptr1dest = (unsigned char *) dest;
	ptr2src = (unsigned char *) src;
	pos = 0;
	if (dest == NULL && src == NULL)
		return (NULL);
	while (pos < n)
	{
		ptr1dest[pos] = ptr2src[pos];
		pos++;
	}
	return (dest);
}
