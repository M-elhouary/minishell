/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: houardi <houardi@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 18:52:49 by houardi           #+#    #+#             */
/*   Updated: 2025/08/14 06:41:13 by houardi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

long	atol_s(const char *str, char **endptr)
{
	long	res;
	int		i;
	int		sign;

	res = 0;
	i = 0;
	sign = 1;
	while (str[i] == 32 || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (str[i] != '\0' && str[i] >= '0' && str[i] <= '9')
	{
		if (sign == 1)
		{
			if (res > (LONG_MAX - (str[i] - 48)) / 10)
				break;
		}
		else
		{
			if (res > (-(LONG_MIN + (str[i] - 48))) / 10)
				break;
		}
		res = res * 10 + (str[i] - 48);
		i++;
	}
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
