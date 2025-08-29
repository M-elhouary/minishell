/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houa <mel-houa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 22:22:06 by mel-houa          #+#    #+#             */
/*   Updated: 2024/11/16 23:35:02 by mel-houa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	const unsigned char	*p;
	unsigned char		uc;

	p = (const unsigned char *) s;
	uc = (unsigned char ) c;
	if (n != 0)
	{
		while (n > 0)
		{
			if (*p == uc)
			{
				return ((void *)p);
			}
			p++;
			n--;
		}
	}
	return (NULL);
}
