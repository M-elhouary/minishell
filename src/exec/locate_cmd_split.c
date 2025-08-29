/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   locate_cmd_split.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: houardi <houardi@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 10:41:56 by houardi           #+#    #+#             */
/*   Updated: 2025/08/27 06:57:24 by houardi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**allocate_paths_array(char *path_env)
{
	char	**paths;
	int		count;

	count = 0;
	while (*path_env)
		if (*path_env++ == ':')
			count++;
	paths = malloc(sizeof(char *) * (count + 2));
	return (paths);
}

void	free_split(char **arr, int size)
{
	while (--size >= 0)
		free(arr[size]);
	free(arr);
}

int	add_path_segment(char **paths, char *start, int index)
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

char	**tokenize_path_string(char *path_copy, char **paths)
{
	char	*start;
	char	*delim;
	int		i;

	i = 0;
	start = path_copy;
	delim = ft_strchr(start, ':');
	while (delim)
	{
		*delim = '\0';
		i = add_path_segment(paths, start, i);
		if (i == -1)
			return (free_split(paths, i), NULL);
		start = delim + 1;
		delim = ft_strchr(start, ':');
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
