/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houa <mel-houa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 22:55:46 by mel-houa          #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2025/07/26 16:17:04 by mel-houa         ###   ########.fr       */
=======
/*   Updated: 2025/07/30 16:56:16 by mel-houa         ###   ########.fr       */
>>>>>>> 002946b (update expand with quote)
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"



<<<<<<< HEAD
// echo $HOME'$HOME' not expand in squotes
// bash-5.2$ echo adf > $a
// bash: $a: ambiguous redirect

static char	*extract_var_name(const char *str, int *i)
=======
static char	*expand_var_in_string(const char *str, t_env *env)
>>>>>>> 002946b (update expand with quote)
{
	char	*result;
	int		i;
	char	*tmp;
	char	*var_value;
	int		start;

<<<<<<< HEAD
	i = 0;
	result = ft_strdup("");//
=======
	result = ft_strdup("");
	i = 0;

>>>>>>> 002946b (update expand with quote)
	while (str[i])
	{
		if (str[i] == '$' && (ft_isalnum(str[i+1]) || str[i+1] == '_'))
		{
			i++;
			start = i;
			while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
				i++;
			tmp = ft_strndup(str + start, i - start);
			var_value = get_env_value(tmp, env);
			if (var_value && *var_value)
				result = ft_strjoin_free(result, var_value);
			else
				result = ft_strjoin_free(result, ft_strdup(""));
			free(tmp);
		}
		else
		{
			tmp = ft_strndup(str + i, 1);
			result = ft_strjoin_free(result, tmp);
			i++;
		}
	}
	return (result);
}

static char	*process_quoted_segment(const char *str, int *i, t_env *env)
{
	char	quote;
	int		start;
	char	*segment;
	char	*result;

	quote = str[*i];
	(*i)++; // Skip opening quote
	start = *i;
	
	while (str[*i] && str[*i] != quote)
		(*i)++;
	
	if (!str[*i])
		return (ft_strdup("")); // Unclosed quote
	
	segment = ft_strndup(str + start, *i - start);
	(*i)++; // Skip closing quote
	
	if (quote == '"')
	{
		// Expand variables inside double quotes
		result = expand_var_in_string(segment, env);
		free(segment);
		return (result);
	}
	else
	{
		// Single quotes: no expansion
		return (segment);
	}
}

static char	*process_unquoted_segment(const char *str, int *i, t_env *env)
{
	int		start;
	char	*segment;
	char	*result;

	start = *i;
	while (str[*i] && str[*i] != '\'' && str[*i] != '"')
		(*i)++;
	
	segment = ft_strndup(str + start, *i - start);
	result = expand_var_in_string(segment, env);
	free(segment);
	return (result);
}

char	*expand_variables(const char *str, t_env *env)
{
	char	*result;
	char	*segment;
	int		i;

	if (!str)
		return (ft_strdup(""));
	
	result = ft_strdup("");
	i = 0;

	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '"')
		{
			segment = process_quoted_segment(str, &i, env);
			result = ft_strjoin_free(result, segment);
		}
		else
		{
			segment = process_unquoted_segment(str, &i, env);
			result = ft_strjoin_free(result, segment);
		}
	}
	return (result);
}