/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houa <mel-houa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 01:01:05 by mel-houa          #+#    #+#             */
/*   Updated: 2024/11/16 20:33:41 by mel-houa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static	int	count_word(const char *s, char sep)
{
	int	count;
	int	found;

	found = 0;
	count = 0;
	while (*s)
	{
		if (*s != sep)
		{
			if (!found)
			{
				count++;
				found = 1;
			}
		}
		else
		{
			found = 0;
		}
		s++;
	}
	return (count);
}

static	char	*malloc_and_file(const char *s, char sep)
{
	int		i;
	int		len;
	char	*ptr1;

	len = 0;
	i = 0;
	if (*s == '\0')
	{
		return (0);
	}
	while (s[len] && s[len] != sep)
		len++;
	ptr1 = (char *)malloc(len + 1);
	if (ptr1 == NULL)
		return (NULL);
	while (i < len)
	{
		ptr1[i] = s[i];
		i++;
	}
	ptr1[i] = '\0';
	return (ptr1);
}

static	void	myfree(char **s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		free(s[i]);
		i++;
	}
	free(s);
}

static char	**skip_sep_move(const char *s, char sep, char **ptr2)
{
	int	index;

	index = 0;
	while (*s)
	{
		while (*s && *s == sep)
			s++;
		if (*s)
		{
			ptr2[index] = malloc_and_file(s, sep);
			if (!ptr2[index])
			{
				myfree(ptr2);
				return (NULL);
			}
			while (*s && *s != sep)
			{
				s++;
			}
			index++;
		}
	}
	ptr2[index] = NULL;
	return (ptr2);
}

char	**ft_split(const char *s, char sep)
{
	char	**ptr2;

	if (!s)
		return (NULL);
	ptr2 = (char **)malloc((count_word(s, sep) + 1) * sizeof(char *));
	if (!ptr2)
		return (NULL);
	return (skip_sep_move(s, sep, ptr2));
}
