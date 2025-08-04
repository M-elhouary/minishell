/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   locate_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: houardi <houardi@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 00:32:24 by houardi           #+#    #+#             */
/*   Updated: 2025/08/03 23:30:22 by houardi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*join_path(char *dir, char *cmd)
{
	size_t	dir_len;
	size_t	cmd_len;
	char	*path;
	
	dir_len = ft_strlen(dir);
	cmd_len = ft_strlen(cmd);
	if (dir_len + cmd_len > PATH_MAX - 2)
		return (NULL);
	path = malloc(dir_len + cmd_len + 2);
	if (!path)
		return (NULL);
	ft_strcpy(path, dir);
	if (dir_len > 0 && dir[dir_len - 1] != '/')
		ft_strcat(path, "/");
	ft_strcat(path, cmd);
	return (path);
}

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

int	ft_count(const char *str)
{
	int	count;

	count = 0;
	while (*str)
		if (*str++ == ':')
			count++;
	return (count);	
}

void	free_split(char **arr, int size)
{
	while (--size >= 0)
		free(arr[size]);
	free(arr);
}

char	**split_path(char *path_env)
{
	char	**paths;
	char	*path_copy;
	char	*start;
	char	*delim;
	int		i;

	if (!path_env || !*path_env)
		return (NULL);
	i = 0;
	path_copy = ft_strdup(path_env);
	if (!path_copy)
		return (NULL);
	paths = malloc(sizeof(char *) * (ft_count(path_env) + 2));
	if (!paths)
		return (NULL);
	start = path_copy;
	while ((delim = ft_strchr(start, ':')))
	{
		*delim = '\0';
		if (*start)
		{
			paths[i] = ft_strdup(start);
			if (!paths[i])
				return (free_split(paths, i), free(path_copy), NULL);
			i++;
		}
		start = delim + 1;
	}
	if (*start)
	{
		paths[i] = ft_strdup(start);
		if (!paths[i])
			return (free_split(paths, i), free(path_copy), NULL);
		i++;
	}
	paths[i] = NULL;
	free(path_copy);
	return (paths);
}

char	*path_cmd(char *cmd)
{
	if (access(cmd, F_OK) == 0)
		return (ft_strdup(cmd));
	return (NULL);
}

char	*full_path(char **paths, char *cmd)
{
	char	*fullpath;
	int		i;

	i = 0;
	while (paths[i])
	{
		if (!*paths[i])
		{
			i++;
			continue;
		}
		fullpath = join_path(paths[i], cmd);
		if (fullpath)
		{
			if (access(fullpath, F_OK) == 0)
			{
				free_paths(paths);
				return (fullpath);
			}
			free(fullpath);
		}
		i++;
	}
	return (NULL);
}

char	*locate_cmd(char *cmd)
{
	char	*path_env;
	char	**paths;
	char	*res;

	if (!cmd || !*cmd)
		return (NULL);
	if (ft_strchr(cmd, '/'))
		return (path_cmd(cmd));
	path_env = getenv("PATH");
	if (!path_env || !*path_env)
		return (NULL);
	paths = split_path(path_env);
	if (!paths)
		return (NULL);
	res = full_path(paths, cmd);
	if (!res)
		free_paths(paths);
	return (res);
}
