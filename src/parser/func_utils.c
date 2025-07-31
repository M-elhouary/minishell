/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   func_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houa <mel-houa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 16:04:05 by mel-houa          #+#    #+#             */
/*   Updated: 2025/07/31 17:08:48 by mel-houa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include "minishell.h"


char	*ft_strjoin_free(char *s1, char *s2)
{
	char	*result;

	result = ft_strjoin(s1, s2);
	free(s1);
	free(s2);
	return (result);
}



int	skip_spaces(const char *line, int *i)
{
	while (line[*i] && is_space(line[*i]))
		(*i)++;
	return (line[*i] != '\0');
}



int	is_metacharacter(char c)
{
	return (c == '|' || c == '<' || c == '>' || c == ' ' 
		|| c == '\t' || c == '\n');
}

int	is_redirection(t_token_type type)
{
	return (type == REDIR_IN || type == REDIR_OUT
		|| type == REDIR_APPEND || type == HEREDOC);
}

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

char *ft_strndup(const char *s, size_t n)
{
    size_t i;
    char *dup;

    if (!s)
        return NULL;
    dup = malloc(n + 1);
    if (!dup)
        return NULL;
    i = 0;
    while (i < n && s[i])
    {
        dup[i] = s[i];
        i++;
    }
    dup[i] = '\0';
    return dup;
}   
int is_space(char c)
{
    return ((c >= 9 && c <= 13 ) || c == 32);
}




int ft_strcmp(const char *s1, const char *s2) 
{
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return (unsigned char)(*s1) - (unsigned char)(*s2);
}


char *ft_strcpy(char *dest, const char *src)
 {
    char *start = dest;

    while (*src) {
        *dest++ = *src++;
    }
    *dest = '\0';  // Add null terminator
    return start;
}
