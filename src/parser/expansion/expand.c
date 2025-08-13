/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houa <mel-houa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 22:55:46 by mel-houa          #+#    #+#             */
/*   Updated: 2025/08/14 00:33:07 by mel-houa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Main function refactored to use helpers
char	*expand_var_in_string(const char *str, t_env *env, t_command *cmd)
{
	int	i;

	char *result, *tmp;
	result = ft_strdup("");
	i = 0;
	while (str[i])
	{
		if (str[i] == '$' && str[i + 1] == '?')
			result = handle_exit_status(&result, &i, cmd);
		else if (str[i] == '$' && (ft_isalnum(str[i + 1]) || str[i + 1] == '_'))
			result = handle_variable(&result, str, &i, env);
		else
		{
			tmp = ft_strndup(str + i, 1);
			result = ft_strjoin_free(result, tmp);
			i++;
		}
	}
	return (result);
}

// Handle quoted substrings  expanding variables if needed
static char	*process_quoted(const char *str, int *i, t_env *env, t_command *cmd)
{
	char	quote;
	int		start;
	char	*expnd;
	char	*result;

	quote = str[*i];
	(*i)++;
	start = *i;
	while (str[*i] && str[*i] != quote)
		(*i)++;
	if (!str[*i])
		return (ft_strdup(""));
	expnd = ft_strndup(str + start, *i - start);
	(*i)++;
	if (quote == '"')
	{
		result = expand_var_in_string(expnd, env, cmd);
		free(expnd);
		return (result);
	}
	else
		return (expnd);
}

// Handle unquoted substrings and  expanding variables
static char	*process_unquoted(const char *str, int *i, t_env *env,
		t_command *cmd)
{
	int		start;
	char	*expnd;
	char	*result;

	start = *i;
	while (str[*i] && str[*i] != '\'' && str[*i] != '"')
		(*i)++;
	expnd = ft_strndup(str + start, *i - start);
	result = expand_var_in_string(expnd, env, cmd);
	free(expnd);
	return (result);
}

//  to process the main loop of expand_variables
//  for expand_variables processes the main loop
static char	*expand_variables_loop(const char *str, t_env *env, int *i_ptr,
		t_command *cmd)
{
	char	*result;
	char	*expnd;
	int		i;

	result = ft_strdup("");
	i = *i_ptr;
	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '"')
		{
			expnd = process_quoted(str, &i, env, cmd);
			result = ft_strjoin_free(result, expnd);
		}
		else
		{
			expnd = process_unquoted(str, &i, env, cmd);
			result = ft_strjoin_free(result, expnd);
		}
	}
	*i_ptr = i;
	return (result);
}

// Expand all variables in a string, handling quotes
char	*expand_variables(const char *str, t_env *env, t_command *cmd)
{
	char *result;
	int i = 0;
	if (!str)
		return (ft_strdup(""));
	result = expand_variables_loop(str, env, &i, cmd);
	return (result);
}