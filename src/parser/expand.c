/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houa <mel-houa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 22:55:46 by mel-houa          #+#    #+#             */
/*   Updated: 2025/07/25 02:00:19 by mel-houa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"


static char	*extract_var_name(const char *str, int *i)
{
	int		start;
	char	*var_name;

	start = *i;
	while (str[*i] && (ft_isalnum(str[*i]) || str[*i] == '_'))
		(*i)++;
	var_name = ft_strndup(str + start, *i - start);
	return (var_name);
}

static char	*append_str(char *result, char *to_append)
{
	char	*new_result;
	char	*tmp;

	if (!to_append)
		return (result);
	tmp = result;
	new_result = ft_strjoin(result, to_append);
	free(tmp);
	free(to_append);
	return (new_result);
}

char	*expand_variables(const char *str, t_env *env)
{
	int		i;
	char	*result;
	char	*var_name;
	char	*var_value;

	i = 0;
	result = ft_strdup("");
	while (str[i])
	{
		if (str[i] == '$' && (ft_isalpha(str[i + 1]) || str[i + 1] == '_'))
		{
			i++;
			var_name = extract_var_name(str, &i);
			var_value = get_env_value(var_name, env);
			free(var_name);
			result = append_str(result, ft_strdup(var_value));
		}
		else
		{
			result = append_str(result, ft_strndup(str + i, 1));
			i++;
		}
	}
	return (result);
}