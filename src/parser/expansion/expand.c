/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houa <mel-houa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 22:55:46 by mel-houa          #+#    #+#             */
/*   Updated: 2025/08/22 10:51:12 by mel-houa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*expand_var_in_string_gc(const char *str, t_env *env, t_command *cmd,
		t_gc *gc)
{
	int	i;

	char *result, *tmp;
	result = gc_strdup(gc, "");
	i = 0;
	while (str[i])
	{
		if (str[i] == '$' && str[i + 1] == '?')
			result = handle_exit_status_gc(&result, &i, cmd, gc);
		else if (str[i] == '$' && (ft_isalpha(str[i + 1]) || str[i + 1] == '_'))
			result = handle_variable_gc(&result, str, &i, env, gc);
		else
		{
			tmp = gc_strndup(gc, str + i, 1);
			result = ft_strjoin_free_gc(result, tmp, gc);
			i++;
		}
	}
	return (result);
}

static char	*process_quoted_gc(const char *str, int *i, t_env *env,
		t_command *cmd, t_gc *gc)
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
		return (gc_strdup(gc, ""));
	expnd = gc_strndup(gc, str + start, *i - start);
	(*i)++;
	if (quote == '"')
	{
		result = expand_var_in_string_gc(expnd, env, cmd, gc);
		return (result);
	}
	else
		return (expnd);
}

static char	*process_unquoted_gc(const char *str, int *i, t_env *env,
		t_command *cmd, t_gc *gc)
{
	int		start;
	char	*expnd;
	char	*result;

	start = *i;
	while (str[*i] && str[*i] != '\'' && str[*i] != '"')
		(*i)++;
	expnd = gc_strndup(gc, str + start, *i - start);
	result = expand_var_in_string_gc(expnd, env, cmd, gc);
	return (result);
}

static char	*expand_variables_loop_gc(const char *str, t_env *env, int *i_ptr,
		t_command *cmd, t_gc *gc)
{
	char	*result;
	char	*expnd;
	int		i;

	result = gc_strdup(gc, "");
	i = *i_ptr;
	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '"')
		{
			expnd = process_quoted_gc(str, &i, env, cmd, gc);
			result = ft_strjoin_free_gc(result, expnd, gc);
		}
		else
		{
			expnd = process_unquoted_gc(str, &i, env, cmd, gc);
			result = ft_strjoin_free_gc(result, expnd, gc);
		}
	}
	*i_ptr = i;
	return (result);
}

char	*expand_variables_gc(const char *str, t_env *env, t_command *cmd,
		t_gc *gc)
{
	char *result;
	int i = 0;
	if (!str)
		return (gc_strdup(gc, ""));
	result = expand_variables_loop_gc(str, env, &i, cmd, gc);
	return (result);
}