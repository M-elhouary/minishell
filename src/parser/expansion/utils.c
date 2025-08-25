/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houa <mel-houa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 00:27:53 by mel-houa          #+#    #+#             */
/*   Updated: 2025/08/24 08:37:30 by mel-houa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*handle_exit_status_gc(char **result, int *i, t_command *cmd, t_gc *gc)
{
	char	*exit_str;
	char	*new_result;

	exit_str = ft_itoa(cmd->status_exit);
	new_result = ft_strjoin_free_gc(*result, gc_strdup(gc, exit_str), gc);
	free(exit_str);
	*i += 2;
	return (new_result);
}

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
char	*handle_variable_gc(char **result, const char *str, int *i, t_env *env,
		t_gc *gc)
{
	int start;
	char *tmp, *var_value, *new_result;

	(*i)++;
	start = *i;
	while (str[*i] && (ft_isalnum(str[*i]) || str[*i] == '_'))
		(*i)++;
	tmp = gc_strndup(gc, str + start, *i - start);
	var_value = get_env_value(tmp, env);

	if (var_value && *var_value)
		new_result = ft_strjoin_free_gc(*result, gc_strdup(gc, var_value), gc);
	else
		new_result = ft_strjoin_free_gc(*result, gc_strdup(gc, ""), gc);

	free(var_value);
	return (new_result);
}