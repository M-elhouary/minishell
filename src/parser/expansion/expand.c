/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houa <mel-houa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 22:55:46 by mel-houa          #+#    #+#             */
/*   Updated: 2025/08/04 23:51:31 by mel-houa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"



// Expand all $VAR in a string using the environment
static char	*expand_var_in_string(const char *str, t_env *env, t_command *cmd)
{
	char	*result;
	int		i;
	char	*tmp;
	char	*var_value;
	int		start;
	int check;

	result = ft_strdup("");//
	i = 0;
	check = 0;
	while (str[i])
	{
		if (str[i] == '$' && (ft_isalnum(str[i + 1]) || str[i + 1] == '_'))
		{
			
			i++;
			if(str[i] == '?')
				 check = 1;
			if(!check)
			{
				start = i;
				while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
					i++;
				tmp = ft_strndup(str + start, i - start);
				var_value = get_env_value(tmp, env);
			}
			else
				var_value = ft_itoa(cmd->status_exit);
			if (var_value && *var_value)
				result = ft_strjoin_free(result, ft_strdup(var_value));
			else
				result = ft_strjoin_free(result, ft_strdup(""));//
			free(tmp);
			free(var_value);
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

// Handle quoted substrings, expanding variables if needed
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

// Handle unquoted substrings, expanding variables
static char	*process_unquoted(const char *str, int *i, t_env *env, t_command *cmd)
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

// Helper function to check if string contains unquoted variables
// Check if a string contains unquoted variables ($VAR)
int	has_unquoted_variables(const char *str)
{
	int	i;
	int	in_quotes;
	char quote_char;

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
		else if (!in_quotes && str[i] == '$' && (ft_isalnum(str[i+1]) || str[i+1] == '_'))
			return (1);
		i++;
	}
	return (0);
}


// Helper to process the main loop of expand_variables
// Helper for expand_variables: processes the main loop
static char *expand_variables_loop(const char *str, t_env *env, int *i_ptr, t_command *cmd)
{
	char *result = ft_strdup("");
	char *expnd;
	int i = *i_ptr;
	while (str[i]) {
		if (str[i] == '\'' || str[i] == '"') {
			expnd = process_quoted(str, &i, env, cmd);
			result = ft_strjoin_free(result, expnd);
		}
		 else {
			expnd = process_unquoted(str, &i, env, cmd);
			result = ft_strjoin_free(result, expnd);
		}
	}
	*i_ptr = i;
	return result;
}

// Expand all variables in a string, handling quotes
char *expand_variables(const char *str, t_env *env, t_command *cmd)
{
	char *result;
	int i = 0;
	if (!str)
		return (ft_strdup(""));
	result = expand_variables_loop(str, env, &i, cmd);
	return result;
}