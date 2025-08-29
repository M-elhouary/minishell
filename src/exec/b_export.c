/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_export.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: houardi <houardi@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 04:15:25 by houardi           #+#    #+#             */
/*   Updated: 2025/08/27 04:43:18 by houardi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	print_all_exports(t_env *env, int fd)
{
	t_env		*current_env;

	current_env = env;
	while (current_env)
	{
		print("declare -x ", fd);
		print(current_env->key, fd);
		if (current_env->exported_only)
			print("\n", fd);
		else if (current_env->content != NULL)
		{
			print("=\"", fd);
			print(current_env->content, fd);
			print("\"", fd);
			print("\n", fd);
		}
		else
		{
			print("\n", fd);
		}
		current_env = current_env->next;
	}
	return (BUILTIN_SUCCESS);
}

int	validate_export_identifier(char *arg, int fd)
{
	int	j;

	if (!ft_isalpha(arg[0]) && arg[0] != '_')
	{
		print("export: `", fd);
		print(arg, fd);
		print("': not a valid identifier\n", fd);
		return (BUILTIN_ERROR);
	}
	j = 1;
	while (arg[j])
	{
		if (!ft_isalpha(arg[j]) && arg[j] != '_'
			&& !(arg[j] >= '0' && arg[j] <= '9'))
		{
			print("export: `", fd);
			print(arg, fd);
			print("': not a valid identifier\n", fd);
			return (BUILTIN_ERROR);
		}
		j++;
	}
	return (BUILTIN_SUCCESS);
}

int	process_export_with_value(char *arg, char *equals, t_env **env, int fd)
{
	char	*value;
	int		result;

	if (ft_strcmp(arg, "=") != 0)
		*equals = '\0';
	value = equals + 1;
	result = validate_export_identifier(arg, fd);
	if (result != BUILTIN_SUCCESS)
	{
		*equals = '=';
		return (result);
	}
	if (set_env_value(env, arg, value) != 0)
	{
		*equals = '=';
		return (BUILTIN_ERROR);
	}
	*equals = '=';
	return (BUILTIN_SUCCESS);
}

int	process_export_argument(char *arg, t_env **env, int fd)
{
	char	*equals;
	int		result;

	equals = ft_strchr(arg, '=');
	if (!equals)
	{
		result = validate_export_identifier(arg, fd);
		if (result != BUILTIN_SUCCESS)
			return (result);
		return (set_env_exported_only(env, arg));
	}
	return (process_export_with_value(arg, equals, env, fd));
}

int	export_c(char **args, t_env **env, int fd)
{
	int			i;
	int			result;

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
