/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houa <mel-houa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 20:14:42 by mel-houa          #+#    #+#             */
/*   Updated: 2024/11/17 21:25:35 by mel-houa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	size_t				i;
	const unsigned char	*fors1;
	const unsigned char	*fors2;

	fors1 = (const unsigned char *)s1;
	fors2 = (const unsigned char *)s2;
	i = 0;
	while (i < n)
	{
		if (fors1[i] != fors2[i])
		{
			return (fors1[i] - fors2[i]);
		}
		i++;
	}
	return (0);
}
/*
int main()
{
	const int s1[10] = {};
	const int s2[20] = {};
	size_t n = 7;
	printf("%d\n", ft_memcmp(s1, s2, n));
	printf("%d\n", memcmp(s1, s2, n));
}
*/
