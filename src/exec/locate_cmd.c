/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   locate_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: houardi <houardi@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 00:32:24 by houardi           #+#    #+#             */
/*   Updated: 2025/08/27 02:44:53 by houardi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_paths(char **paths)
{
	int	i;

	i = 0;
	if (!paths)
		return;
	while (paths[i])
	{
		free(paths[i]);
		i++;
	}
	free(paths);
}


char	*path_cmd(char *cmd)
{
	if (access(cmd, F_OK) == 0)
		return (ft_strdup(cmd));
	return (NULL);
}

char	*locate_cmd(char *cmd, t_env *env)
{
	char	*path_env;
	char	**paths;
	char	*res;

	if (!cmd || !*cmd)
		return (NULL);
	if (ft_strcmp(cmd, "..") == 0)
		return (NULL);
	if (ft_strchr(cmd, '/'))
		return (path_cmd(cmd));
	path_env = get_env_value("PATH", env);
	if (!path_env || !*path_env)
	{
		free(path_env);
		return (NULL);
	}
	paths = split_path(path_env);
	free(path_env);
	if (!paths)
		return (NULL);
	res = full_path(paths, cmd);
	if (!res)
		free_paths(paths);
	return (res);
}
