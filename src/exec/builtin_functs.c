/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_functs.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: houardi <houardi@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 03:56:15 by houardi           #+#    #+#             */
/*   Updated: 2025/08/27 03:51:54 by houardi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	pwd_c(t_env *env)
{
	printf("%s\n", get_env_value("PWD", env));
	return (BUILTIN_SUCCESS);
}

int	env_c(t_env *env, int fd)
{
	t_env	*current = env;

	while (current)
	{
		if (!current->exported_only && current->content)
		{	
			write(fd, current->key, ft_strlen(current->key));
			write(fd, "=", 1);
			write(fd, current->content, ft_strlen(current->content));
			write(fd, "\n", 1);
		}
		current = current->next;
	}
	return (BUILTIN_SUCCESS);
}

int	unset_c(char **args, t_env **env)
{
	int	i;
	int	res;

	res = BUILTIN_SUCCESS;
	i = 1;
	while (args[i])
	{
		unset_env_value(env, args[i]);
		i++;
	}
	return (res);
}

int	exit_c(char **args, int fd, int exit_status, int print_exit)
{
	long	exit_code;
	char	*endptr;

	if (print_exit)
		print("exit\n", 2);
	if (args[1])
	{
		if (args[2])
		{
			print("exit: too many arguments\n", fd);
			return (1);
		}
		exit_code = atol_s(args[1], &endptr);
		if (*endptr != '\0' || args[1][0] == '\0')
		{
			print("minishell: exit: ", fd);
			print(args[1], fd);
			print(": numeric argument required\n", fd);
			exit(2);
		}
		exit(exit_code);
	}
	else
		exit_code = exit_status;
	exit(exit_code);
}
