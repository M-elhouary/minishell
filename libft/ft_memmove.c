/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houa <mel-houa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 17:23:17 by mel-houa          #+#    #+#             */
/*   Updated: 2024/11/17 21:48:48 by mel-houa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	size_t			i;
	unsigned char	*fordest;
	unsigned char	*forsrc;

	fordest = (unsigned char *)dest;
	forsrc = (unsigned char *)src;
	if (dest == NULL && src == NULL)
		return (NULL);
	i = 0;
	if (dest > src && dest < src + n)
	{
		i = n;
		while (i--)
			fordest[i] = forsrc[i];
	}
	else
	{
		while (n--)
		{
			fordest[i] = forsrc[i];
			i++;
		}
	}
	return (dest);
}
/*
int main()
{
	char src[20] = "mohamed";
	size_t n = 6;
	printf("%s\n", ft_memmove(src+2, src, n));
	char src1[20] = "mohamed";
	printf("%s\n", memmove(src1+2, src1, n));
}
*/
