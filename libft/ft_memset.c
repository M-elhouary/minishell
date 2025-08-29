/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houa <mel-houa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 21:09:47 by mel-houa          #+#    #+#             */
/*   Updated: 2024/11/17 23:49:47 by mel-houa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *s, int value, size_t n)
{
	size_t			i;
	unsigned char	*ptr;

	ptr = (unsigned char *) s;
	i = 0;
	while (n > 0)
	{
		ptr[i] = (unsigned char)value;
		i++;
		n--;
	}
	return (s);
}
