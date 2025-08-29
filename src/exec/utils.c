/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houa <mel-houa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 18:52:49 by houardi           #+#    #+#             */
/*   Updated: 2025/08/27 22:31:01 by mel-houa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	skipspaces(int i, char *str)
{
	while (str[i] == 32 || (str[i] >= 9 && str[i] <= 13))
		i++;
	return (i);
}

long	check_overflow(char *str, int *i, int sign)
{
	long	res;

	res = 0;
	while (str[*i] != '\0' && str[*i] >= '0' && str[*i] <= '9')
	{
		if (sign == 1)
		{
			if (res > (LONG_MAX - (str[*i] - 48)) / 10)
				break ;
		}
		else
		{
			if (res > (-(LONG_MIN + (str[*i] - 48))) / 10)
				break ;
		}
		res = res * 10 + (str[*i] - 48);
		(*i)++;
	}
	return (res);
}

long	atol_s(char *str, char **endptr)
{
	long	res;
	int		i;
	int		sign;

	res = 0;
	i = 0;
	sign = 1;
	i = skipspaces(i, str);
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	res = check_overflow(str, &i, sign);
	i = skipspaces(i, str);
	*endptr = (char *)(str + i);
	return (res * sign);
}

char	*ft_strcat(char *dst, char *src)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (dst[i])
		i++;
	while (src[j])
	{
		dst[i] = src[j];
		i++;
		j++;
	}
	dst[i] = '\0';
	return (dst);
}

void	print(char *s, int fd)
{
	write(fd, s, ft_strlen(s));
}
