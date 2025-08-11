/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: houardi <houardi@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 04:25:53 by houardi           #+#    #+#             */
/*   Updated: 2025/08/11 08:25:27 by houardi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	err_(char *str)
{
	printf("%s\n", str);
	return (BUILTIN_ERROR);
}

int	cd_c(char **args, t_env **env, int fd)
{
	char	*path;
	char	*oldpwd;
	char	cwd_buff[PATH_MAX];

	if (args[1] && ft_strcmp(args[1], "-") == 0)
	{
		path = get_env_value("OLDPWD", *env);
		if (!path)
			return (err_("cd: OLDPWD not set"));
		print(path, fd);
		print("\n", fd);
	}
	else if (!args[1])
	{
		path = get_env_value("HOME", *env);
		if (!path)
			return (err_("cd: Home not set"));
	}
	else if (args[2])
		return (err_("cd: too many arguments"));
	else
		path = args[1];
	if (getcwd(cwd_buff, sizeof(cwd_buff)))
		oldpwd = ft_strdup(cwd_buff);
	else
		oldpwd = NULL;
	if (chdir(path) == -1)
	{
		print(path, fd);
		print("\n", fd);
		perror("cd");
		free(oldpwd);
		return (BUILTIN_ERROR);
	}
	if (oldpwd)
	{
		set_env_value(env, "OLDPWD", cwd_buff);
		free(oldpwd);
	}
	if (getcwd(cwd_buff, sizeof(cwd_buff)))
		set_env_value(env, "PWD", cwd_buff);
	return (BUILTIN_SUCCESS);
}
