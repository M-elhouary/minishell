/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: houardi <houardi@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 04:15:25 by houardi           #+#    #+#             */
/*   Updated: 2025/08/17 05:40:18 by houardi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	print_all_exports(t_env *env, int fd)
{
	t_env	*current;

	current = env;
	while (current)
	{
		print("declare -x ", fd);
		print(current->key, fd);
		print("=\"", fd);
		print(current->content, fd);
		print("\"\n", fd);
		current = current->next;
	}
	return (BUILTIN_SUCCESS);
}

static int	validate_export_identifier(char *key, char *original_arg, int fd)
{
	int	j;

	if (!ft_isalpha(key[0]) && key[0] != '_')
	{
		print("export: `", fd);
		print(original_arg, fd);
		print("': not a valid identifier\n", fd);
		return (BUILTIN_ERROR);
	}
	j = 1;
	while (key[j])
	{
		if (!ft_isalpha(key[j]) && key[j] != '_' &&
			!(key[j] >= '0' && key[j] <= '9'))
		{
			print("export: `", fd);
			print(original_arg, fd);
			print("': not a valid identifier\n", fd);
			return (BUILTIN_ERROR);
		}
		j++;
	}
	return (BUILTIN_SUCCESS);
}

static int	process_export_argument(char *arg, t_env **env, int fd)
{
	char	*equals;
	char	*key;
	char	*value;
	int		result;

	equals = ft_strchr(arg, '=');
	if (!equals)
	{
		result = validate_export_identifier(arg, arg, fd);
		return (result);
	}
	if (ft_strcmp(arg, "=") != 0)
		*equals = '\0';
	key = arg;
	value = equals + 1;
	result = validate_export_identifier(key, arg, fd);
	if (result != BUILTIN_SUCCESS)
	{
		*equals = '=';
		return (result);
	}
	if (set_env_value(env, key, value) != 0)
	{
		*equals = '=';
		return (BUILTIN_ERROR);
	}
	*equals = '=';
	return (BUILTIN_SUCCESS);
}

int	export_c(char **args, t_env **env, int fd)
{
	int	i;
	int	result;
	
	if (!args[1])
		return (print_all_exports(*env, fd));
	
	i = 1;
	while (args[i])
	{
		result = process_export_argument(args[i], env, fd);
		if (result != BUILTIN_SUCCESS)
			return (result);
		i++;
	}
	return (BUILTIN_SUCCESS);
}
