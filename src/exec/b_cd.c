/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_cd.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: houardi <houardi@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 04:25:53 by houardi           #+#    #+#             */
/*   Updated: 2025/08/26 07:06:11 by houardi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	err_(char *str)
{
	printf("%s\n", str);
	return (BUILTIN_ERROR);
}

// `cd` with no arguments
char	*handle_cd_home(t_env **env)
{
	char	*path;

	path = get_env_value("HOME", *env);
	if (!path)
	{
		err_("cd: Home not set");
		return (NULL);
	}
	return (path);
}

// path selector
char	*get_cd_target(char **args, t_env **env)
{
	if (!args[1])
		return (handle_cd_home(env));
	if (args[2])
	{
		err_("cd: too many arguments");
		return (NULL);
	}
	return (args[1]);
}

// Updates OLDPWD and PWD after changing directory
void	update_pwd_vars(t_env **env)
{
	char	cwd_buff[PATH_MAX];

	if (getcwd(cwd_buff, sizeof(cwd_buff)))
		set_env_value(env, "PWD", cwd_buff);
}

int	cd_c(char **args, t_env **env, int fd)
{
	char	*path;

	(void)fd;
	path = get_cd_target(args, env);
	if (!path)
		return (BUILTIN_ERROR);
	if (chdir(path) == -1)
	{
		perror("cd");
		return (BUILTIN_ERROR);
	}
	update_pwd_vars(env);
	return (BUILTIN_SUCCESS);
}
