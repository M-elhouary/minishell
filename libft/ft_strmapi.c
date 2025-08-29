/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houa <mel-houa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 23:53:35 by mel-houa          #+#    #+#             */
/*   Updated: 2024/11/11 23:59:40 by mel-houa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	int		lenofs;
	int		i;
	char	*newstr;

	if (!s || !f)
		return (NULL);
	lenofs = ft_strlen(s);
	newstr = malloc(lenofs + 1);
	if (!newstr)
		return (NULL);
	i = 0;
	while (i < lenofs)
	{
		newstr[i] = f(i, s[i]);
		i++;
	}
	newstr[i] = '\0';
	return (newstr);
}
