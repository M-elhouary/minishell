/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houa <mel-houa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 15:30:55 by mel-houa          #+#    #+#             */
/*   Updated: 2024/11/17 21:56:19 by mel-houa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	dlen;
	size_t	slen;
	size_t	i;

	if (!dst && !src)
		return (0);
	if (size == 0)
		return (ft_strlen(src));
	dlen = ft_strlen(dst);
	slen = ft_strlen(src);
	if (size <= dlen)
		return (size + slen);
	i = 0;
	while (src[i] && (dlen + i) < (size - 1))
	{
		dst[dlen + i] = src[i];
		i++;
	}
	dst[dlen + i] = '\0';
	return (dlen + slen);
}
/*
int main()
{
	char dest[10] = NULL;
	char src[10] = NULL;
	size_t n= 7;
	printf("%d\n", ft_strlcat(dest, src, n));
	printf("%d\n", strlcat(dest, src, n));
}
*/
