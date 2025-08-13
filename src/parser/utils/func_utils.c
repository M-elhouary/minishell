/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   func_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houa <mel-houa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 16:04:05 by mel-houa          #+#    #+#             */
/*   Updated: 2025/08/14 00:36:02 by mel-houa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_error(char *error, char *detail)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	if (detail)
	{
		ft_putstr_fd(detail, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
	}
	ft_putstr_fd(error, STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
}

char	*ft_strndup(const char *s, size_t n)
{
	size_t	i;
	char	*dup;

	if (!s)
		return (NULL);
	dup = malloc(n + 1);
	if (!dup)
		return (NULL);
	i = 0;
	while (i < n && s[i])
	{
		dup[i] = s[i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
}
int	is_space(char c)
{
	return ((c >= 9 && c <= 13) || c == 32);
}

int	ft_strcmp(const char *s1, const char *s2)
{
	while (*s1 && (*s1 == *s2))
	{
		s1++;
		s2++;
	}
	return (unsigned char)(*s1) - (unsigned char)(*s2);
}

char	*ft_strcpy(char *dest, const char *src)
{
	char	*start;

	start = dest;
	while (*src)
	{
		*dest++ = *src++;
	}
	*dest = '\0'; // Add null terminator
	return (start);
}

int	has_unclosed_quote(const char *line)
{
	int		i;
	char	quote;

	i = 0;
	quote = 0;
	while (line[i])
	{
		// If not in a quote and found a quote character, enter quote mode
		if (!quote && (line[i] == '\'' || line[i] == '"'))
			quote = line[i];
		// If in a quote and found matching quote character, exit quote mode
		else if (quote && line[i] == quote)
			quote = 0;
		i++;
	}
	// If still in a quote at end of line, there's an unclosed quote
	return (quote != 0);
}

int	has_quotes(const char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (0);
	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '\"')
			return (1);
		i++;
	}
	return (0);
}
