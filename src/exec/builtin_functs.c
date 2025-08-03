/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_functs.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: houardi <houardi@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 03:56:15 by houardi           #+#    #+#             */
/*   Updated: 2025/08/03 23:25:28 by houardi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	echo_c(char **args)
{
	int	i;
	int	newline;

	i = 1;
	newline = 1;
	while (args[i] && ft_strcmp(args[i], "-n") == 0)
	{
		newline = 0;
		i++;
	}
	if (!args[i])
	{
		if (newline)
			printf("\n");
		return (BUILTIN_SUCCESS);
	}
	while (args[i])
	{
		printf("%s", args[i]);
		if (args[i + 1])
			printf(" ");
		i++;
	}
	if (newline)
		printf("\n");
	return (BUILTIN_SUCCESS);
}

int	pwd_c(void)
{
	char	*cwd;
	
	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		perror("pwd");
		return (BUILTIN_ERROR);
	}
	printf("%s\n", cwd);
	free(cwd);
	return (BUILTIN_SUCCESS);
}

int	env_c(t_env *env)
{
	t_env	*current = env;

	while (current)
	{
		printf("%s=%s\n", current->key, current->content);
		current = current->next;
	}
	return (BUILTIN_SUCCESS);
}

int	unset_c(char **args, t_env **env)
{
	int	i;
	int	res;

	res = BUILTIN_SUCCESS;
	if (!args[1])
	{
		printf("unset: not enough arguments\n");
		return (BUILTIN_ERROR);
	}
	i = 0;
	while (args[i])
	{
		if (!ft_isalpha(args[i][0]) && args[i][0] != '_')
		{
			printf("unset: `%s': not a valid identifier\n", args[i]);
			res = BUILTIN_ERROR;
		}
		else
			unset_env_value(env, args[i]);
		i++;
	}
	return (res);
}

int	exit_c(char **args)
{
	long	exit_code;
	char	*endptr;

	printf("exit\n");
	exit_code = 0;
	if (args[1])
	{
		if (args[2])
		{
			printf("exit: too many arguments\n");
			return (BUILTIN_ERROR);
		}
	}
	exit_code = atol_s(args[1], &endptr);
	if (*endptr != '\0')
	{
		printf("minishell: exit: %s: numeric argument required\n", args[1]);
		exit(2);
	}
	exit(exit_code & 255);
}
