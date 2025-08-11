/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: houardi <houardi@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 04:15:25 by houardi           #+#    #+#             */
/*   Updated: 2025/08/11 08:39:21 by houardi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	export_c(char **args, t_env **env, int fd)
{
	char	*equals;
	char	*key;
	char	*value;
	t_env	*current;
	int		i;
	int		j;
	
	i = 1;
	if (!args[1])
	{
		current = *env;
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
	while (args[i])
	{
		equals = ft_strchr(args[i], '=');
		if (!equals)
		{
			i++;
			continue;
		}
		*equals = '\0';
		key = args[i];
		value = equals + 1;
		if (!ft_isalpha(key[0]) && key[0] != '_')
		{
			print("export: `", fd);
			print(args[i], fd);
			print("': not a valid identifier\n", fd);
			*equals = '=';
			return (BUILTIN_ERROR);
		}
		j = 1;
		while (key[j])
		{
			if (!ft_isalpha(key[j]) && key[j] != '_' &&
				!(key[j] >= '0' && key[j] <= '9'))
			{
				print("export: `", fd);
				print(args[i], fd);
				print("': not a valid identifier\n", fd);
				*equals = '=';
				return (BUILTIN_ERROR);
			}
			j++;
		}
		
		if (set_env_value(env, key, value) != 0)
		{
			*equals = '=';
			return (BUILTIN_ERROR);
		}
		*equals = '=';
		i++;
	}
	return (BUILTIN_SUCCESS);
}
