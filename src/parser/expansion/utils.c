/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houa <mel-houa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 00:27:53 by mel-houa          #+#    #+#             */
/*   Updated: 2025/08/14 00:33:19 by mel-houa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Helper function to handle exit status expansion
char	*handle_exit_status(char **result, int *i, t_command *cmd)
{
	char	*exit_str;
	char	*new_result;

	exit_str = ft_itoa(cmd->status_exit);
	new_result = ft_strjoin_free(*result, exit_str);
	*i += 2; // Skip both $ and ?
	return (new_result);
}

// function to check if string contains unquoted variables
// Check if a string contains unquoted variables ($VAR)
int	has_unquoted_variables(const char *str)
{
	int		i;
	int		in_quotes;
	char	quote_char;

	i = 0;
	in_quotes = 0;
	quote_char = 0;
	while (str[i])
	{
		if (!in_quotes && (str[i] == '\'' || str[i] == '"'))
		{
			in_quotes = 1;
			quote_char = str[i];
		}
		else if (in_quotes && str[i] == quote_char)
		{
			in_quotes = 0;
			quote_char = 0;
		}
		else if (!in_quotes && str[i] == '$' && (ft_isalnum(str[i + 1]) || str[i
				+ 1] == '_'))
			return (1);
		i++;
	}
	return (0);
}
// Helper function to handle variable expansion
char	*handle_variable(char **result, const char *str, int *i, t_env *env)
{
	int start;
	char *tmp, *var_value, *new_result;

	(*i)++;
	start = *i;
	while (str[*i] && (ft_isalnum(str[*i]) || str[*i] == '_'))
		(*i)++;
	tmp = ft_strndup(str + start, *i - start);
	var_value = get_env_value(tmp, env);

	if (var_value && *var_value)
		new_result = ft_strjoin_free(*result, ft_strdup(var_value));
	else
		new_result = ft_strjoin_free(*result, ft_strdup(""));

	free(tmp);
	free(var_value);
	return (new_result);
}