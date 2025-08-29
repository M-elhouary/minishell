/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houa <mel-houa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 03:08:26 by mel-houa          #+#    #+#             */
/*   Updated: 2024/11/11 23:56:24 by mel-houa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	count_len(long int n)
{
	int	count;

	count = 0;
	if (n == 0)
	{
		count++;
		return (count);
	}
	if (n < 0)
	{
		n *= -1;
		count++;
	}
	while (n > 0)
	{
		n = n / 10;
		count++;
	}
	return (count);
}

static void	check_zero(int len, char *str)
{
	str[0] = '0';
	str[len] = '\0';
}

char	*ft_itoa(int nb)
{
	char		*str;
	int			len;
	long int	n;

	n = nb;
	len = count_len(n);
	str = malloc(len + 1);
	if (!str)
		return (NULL);
	if (n < 0)
	{
		str[0] = '-';
		n *= -1;
	}
	if (n == 0)
		check_zero(len, str);
	str[len] = '\0';
	while (n > 0)
	{
		str[len - 1] = n % 10 + '0';
		n = n / 10;
		len--;
	}
	return (str);
}
