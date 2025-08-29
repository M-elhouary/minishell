/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houa <mel-houa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 16:06:48 by mel-houa          #+#    #+#             */
/*   Updated: 2024/11/13 22:21:00 by mel-houa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	count_len(long int n)
{
	int	count;

	count = 0;
	if (n == 0)
		return (1);
	if (n < 0)
	{
		count++;
		n = n * -1;
	}
	while (n != 0)
	{
		n = n / 10;
		count++;
	}
	return (count);
}

static void	handle_negative(long int *nb, char *swet)
{
	if (*nb < 0)
	{
		swet[0] = '-';
		*nb = -(*nb);
	}
}

static void	convert_to_string(long int nb, char *swet, int len)
{
	while (nb > 0)
	{
		swet[len - 1] = nb % 10 + '0';
		nb = nb / 10;
		len--;
	}
}

static void	printcontent(char *arr, int fd)
{
	int	index;

	index = 0;
	while (arr[index])
	{
		write (fd, &arr[index], 1);
		index++;
	}
}

void	ft_putnbr_fd(int n, int fd)
{
	long int	nb;
	int			len;
	char		swet[12];

	nb = n;
	len = count_len(nb)+1;
	handle_negative(&nb, swet);
	if (n == 0)
	{
		swet[0] = '0';
		swet[1] = '\0';
		printcontent(swet, fd);
		return ;
	}
	swet[len - 1] = '\0';
	len = len - 1;
	convert_to_string(nb, swet, len);
	printcontent(swet, fd);
}
