/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_echo.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: houardi <houardi@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 08:33:19 by houardi           #+#    #+#             */
/*   Updated: 2025/08/26 08:41:35 by houardi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_n(char **args, int *i, int *newline)
{
	int	j;

	j = 1;
	while (args[*i] && args[*i][j - 1] == '-' && args[*i][j] && args[*i][j] != '-')
	{
		while (args[*i][j])
		{
			if (args[*i][j] == 'n')
				j++;
			else
				break;
		}
		if (args[*i][j] != 'n' && args[*i][j])
			break;
		j = 1;
		*newline = 0;
		(*i)++;
	}
}

int	echo_c(char **args, int fd)
{
	int	i;
	int	newline;

	i = 1;
	newline = 1;
	handle_n(args, &i, &newline);
	if (!args[i])
	{
		if (newline)
			write(fd, "\n", 1);
		return (BUILTIN_SUCCESS);
	}
	while (args[i])
	{
		write(fd, args[i], ft_strlen(args[i]));
		if (args[i + 1])
			write(fd, " ", 1);
		i++;
	}
	if (newline)
		write(fd, "\n", 1);
	return (BUILTIN_SUCCESS);
}
