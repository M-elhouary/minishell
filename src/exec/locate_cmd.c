/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   locate_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: houardi <houardi@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 00:32:24 by houardi           #+#    #+#             */
/*   Updated: 2025/08/12 01:24:32 by houardi          ###   ########.fr       */
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

static char	**allocate_paths_array(char *path_env)
{
	char	**paths;
	int		count;

	count = ft_count(path_env) + 2;
	paths = malloc(sizeof(char *) * count);
	return (paths);
}

static int	add_path_segment(char **paths, char *start, int index)
{
	if (*start)
	{
		paths[index] = ft_strdup(start);
		if (!paths[index])
			return (-1);
		return (index + 1);
	}
	return (index);
}

static char	**tokenize_path_string(char *path_copy, char **paths)
{
	char	*start;
	char	*delim;
	int		i;

	i = 0;
	start = path_copy;
	while ((delim = ft_strchr(start, ':')))
	{
		*delim = '\0';
		i = add_path_segment(paths, start, i);
		if (i == -1)
			return (free_split(paths, i), NULL);
		start = delim + 1;
	}
	i = add_path_segment(paths, start, i);
	if (i == -1)
		return (free_split(paths, i), NULL);
	paths[i] = NULL;
	return (paths);
}

char	**split_path(char *path_env)
{
	char	**paths;
	char	*path_copy;
	char	**result;

	if (!path_env || !*path_env)
		return (NULL);
	path_copy = ft_strdup(path_env);
	if (!path_copy)
		return (NULL);
	paths = allocate_paths_array(path_env);
	if (!paths)
		return (free(path_copy), NULL);
	result = tokenize_path_string(path_copy, paths);
	free(path_copy);
	return (result);
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

char	*locate_cmd(char *cmd, t_env *env)
{
	char	*path_env;
	char	**paths;
	char	*res;

	if (!cmd || !*cmd)
		return (NULL);
	if (ft_strchr(cmd, '/'))
		return (path_cmd(cmd));
	path_env = get_env_value("PATH", env);
	if (!path_env || !*path_env)
	{
		free(path_env);
		// When PATH is unset, treat command as relative path
		return (ft_strdup(cmd));
	}
	paths = split_path(path_env);
	free(path_env);
	if (!paths)
		return (NULL);
	res = full_path(paths, cmd);
	// full_path already frees paths on success, only free on failure
	if (!res)
		free_paths(paths);
	return (res);
}
