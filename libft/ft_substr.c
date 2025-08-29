/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houa <mel-houa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/02 21:10:06 by mel-houa          #+#    #+#             */
/*   Updated: 2024/11/18 00:25:07 by mel-houa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char			*newstring;
	unsigned int	i;

	i = 0;
	if (start >= ft_strlen(s))
		return (ft_strdup(""));
	if (start + len > ft_strlen(s))
		len = ft_strlen(s) - start;
	newstring = (char *)malloc(len + 1);
	if (newstring == NULL)
		return (NULL);
	while (i < len && s[start + i] != '\0')
	{
		newstring[i] = s[start + i];
		i++;
	}
	newstring[i] = '\0';
	return (newstring);
}
