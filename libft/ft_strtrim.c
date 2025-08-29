/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houa <mel-houa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/03 14:37:47 by mel-houa          #+#    #+#             */
/*   Updated: 2024/11/12 00:00:14 by mel-houa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	check_first(char const *s1, char const *set)
{
	int	count_first;
	int	j;
	int	found;

	count_first = 0;
	while (s1[count_first] != '\0')
	{
		j = 0;
		found = 0;
		while (set[j] != '\0')
		{
			if (set[j] == s1[count_first])
			{
				found = 1;
				break ;
			}
			j++;
		}
		if (!found)
			break ;
		count_first++;
	}
	return (count_first);
}

static int	check_last(char const *s1, char const *set, int len_s1)
{
	int	count_last;
	int	i;

	count_last = 0;
	while (len_s1 > 0)
	{
		i = 0;
		while (set[i] != '\0')
		{
			if (set[i] == s1[len_s1 - 1])
			{
				count_last++;
				len_s1--;
				break ;
			}
			i++;
		}
		if (set[i] == '\0')
			break ;
	}
	return (count_last);
}

static char	*handle_empty(int len_s1, int first, int last)
{
	char	*new_str;

	if (first >= len_s1 - last)
	{
		new_str = (char *)malloc(1);
		if (!new_str)
			return (NULL);
		new_str[0] = '\0';
		return (new_str);
	}
	return (NULL);
}

static char	*trim_string(char const *s1, int first, int last, int len_s1)
{
	char	*new_str;
	int		i;

	i = 0;
	new_str = (char *)malloc(len_s1 - first - last + 1);
	if (!new_str)
		return (NULL);
	while (first < len_s1 - last)
	{
		new_str[i] = s1[first];
		first++;
		i++;
	}
	new_str[i] = '\0';
	return (new_str);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	int		first;
	int		last;
	int		len_s1;
	char	*new_str;

	if (!s1 || !set)
		return (NULL);
	len_s1 = ft_strlen(s1);
	first = check_first(s1, set);
	last = check_last(s1, set, len_s1);
	new_str = handle_empty(len_s1, first, last);
	if (new_str)
		return (new_str);
	return (trim_string(s1, first, last, len_s1));
}
