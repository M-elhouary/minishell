/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   locate_cmd_path.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houa <mel-houa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 10:41:50 by houardi           #+#    #+#             */
/*   Updated: 2025/08/27 23:03:15 by mel-houa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*join_path(char *dir, char *cmd)
{
	t_clean	**cleanchild;
	size_t	dir_len;
	size_t	cmd_len;
	char	*path;

	cleanchild = grepclean();
	dir_len = ft_strlen(dir);
	cmd_len = ft_strlen(cmd);
	if (dir_len + cmd_len > PATH_MAX - 2)
		return (NULL);
	path = gc_malloc((*cleanchild)->gc, dir_len + cmd_len + 2);
	if (!path)
		return (NULL);
	ft_strcpy(path, dir);
	if (dir_len > 0 && dir[dir_len - 1] != '/')
		ft_strcat(path, "/");
	ft_strcat(path, cmd);
	return (path);
}

int	is_valid_executable(char *path)
{
	struct stat	st;

	if (access(path, F_OK) != 0)
		return (0);
	if (stat(path, &st) != 0)
		return (0);
	if (!S_ISREG(st.st_mode))
		return (0);
	if (access(path, X_OK) != 0)
		return (0);
	return (1);
}

char	*find_executable(char **paths, char *cmd)
{
	char	*fullpath;
	int		i;

	i = 0;
	while (paths[i])
	{
		if (!*paths[i])
		{
			i++;
			continue ;
		}
		fullpath = join_path(paths[i], cmd);
		if (fullpath && is_valid_executable(fullpath))
			return (fullpath);
		i++;
	}
	return (NULL);
}

char	*full_path(char **paths, char *cmd)
{
	char	*result;

	result = find_executable(paths, cmd);
	if (result)
	{
		free_paths(paths);
		return (result);
	}
	return (NULL);
}
