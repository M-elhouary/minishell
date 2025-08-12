/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: houardi <houardi@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 04:25:53 by houardi           #+#    #+#             */
/*   Updated: 2025/08/12 02:23:28 by houardi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	err_(char *str)
{
	printf("%s\n", str);
	return (BUILTIN_ERROR);
}

// `cd -`
static char	*handle_cd_dash(t_env **env, int fd)
{
	char	*path;

	path = get_env_value("OLDPWD", *env);
	if (!path)
	{
		err_("cd: OLDPWD not set");
		return (NULL);
	}
	print(path, fd);
	print("\n", fd);
	return (path);
}

// `cd` with no arguments
static char	*handle_cd_home(t_env **env)
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
static char	*get_cd_target(char **args, t_env **env, int fd)
{
	if (args[1] && ft_strcmp(args[1], "-") == 0)
		return (handle_cd_dash(env, fd));
	if (!args[1])
		return (handle_cd_home(env));
	if (args[2])
	{
		err_("cd: too many arguments");
		return (NULL);
	}
	return (args[1]);
}

static char	*save_oldpwd(void)
{
	char	cwd_buff[PATH_MAX];

	if (getcwd(cwd_buff, sizeof(cwd_buff)))
		return (ft_strdup(cwd_buff));
	return (NULL);
}

// Updates OLDPWD and PWD after changing directory
static void	update_pwd_vars(t_env **env, char *oldpwd)
{
	char	cwd_buff[PATH_MAX];

	if (oldpwd)
	{
		set_env_value(env, "OLDPWD", oldpwd);
		free(oldpwd);
	}
	if (getcwd(cwd_buff, sizeof(cwd_buff)))
		set_env_value(env, "PWD", cwd_buff);
}

int	cd_c(char **args, t_env **env, int fd)
{
	char	*path;
	char	*oldpwd;

	path = get_cd_target(args, env, fd);
	if (!path)
		return (BUILTIN_ERROR);
	oldpwd = save_oldpwd();
	if (chdir(path) == -1)
	{
		perror("cd");
		free(oldpwd);
		return (BUILTIN_ERROR);
	}
	update_pwd_vars(env, oldpwd);
	return (BUILTIN_SUCCESS);
}

// int	cd_c(char **args, t_env **env, int fd)
// {
// 	char	*path;
// 	char	*oldpwd;
// 	char	cwd_buff[PATH_MAX];

// 	if (args[1] && ft_strcmp(args[1], "-") == 0)
// 	{
// 		path = get_env_value("OLDPWD", *env);
// 		if (!path)
// 			return (err_("cd: OLDPWD not set"));
// 		print(path, fd);
// 		print("\n", fd);
// 	}
// 	else if (!args[1])
// 	{
// 		path = get_env_value("HOME", *env);
// 		if (!path)
// 			return (err_("cd: Home not set"));
// 	}
// 	else if (args[2])
// 		return (err_("cd: too many arguments"));
// 	else
// 		path = args[1];
// 	if (getcwd(cwd_buff, sizeof(cwd_buff)))
// 		oldpwd = ft_strdup(cwd_buff);
// 	else
// 		oldpwd = NULL;
// 	if (chdir(path) == -1)
// 	{
// 		perror("cd");
// 		free(oldpwd);
// 		return (BUILTIN_ERROR);
// 	}
// 	if (oldpwd)
// 	{
// 		set_env_value(env, "OLDPWD", cwd_buff);
// 		free(oldpwd);
// 	}
// 	if (getcwd(cwd_buff, sizeof(cwd_buff)))
// 		set_env_value(env, "PWD", cwd_buff);
// 	return (BUILTIN_SUCCESS);
// }
