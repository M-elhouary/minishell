/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houa <mel-houa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 11:14:06 by mel-houa          #+#    #+#             */
/*   Updated: 2024/11/18 00:25:00 by mel-houa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_bzero(void *s, size_t n)
{
	size_t			pos;
	unsigned char	*ptr;

	ptr = (unsigned char *)s;
	pos = 0;
	while (pos < n)
	{
		ptr[pos] = 0;
		pos++;
	}
}
