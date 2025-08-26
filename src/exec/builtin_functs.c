/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_functs.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hayabusa <hayabusa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 03:56:15 by houardi           #+#    #+#             */
/*   Updated: 2025/08/25 13:14:14 by hayabusa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	echo_c(char **args, int fd)
{
	int	i;
	int	newline;

	i = 1;
	newline = 1;
	int j = 1;
	while (args[i] && args[i][j - 1] == '-' && args[i][j] && args[i][j] != '-')
	{
		while (args[i][j])
		{
			if (args[i][j] == 'n')
				j++;
			else
				break;
		}
		if (args[i][j] != 'n' && args[i][j])
			break;
		j = 1;
		newline = 0;
		i++;
	}
	if (!args[i])
	{
		if (newline)
			write(fd, "\n", 1);
		return (BUILTIN_SUCCESS);
	}
	while (args[i])
	{
		write(fd, args[i], ft_strlen(args[i]));
		if (args[i + 1])
			write(fd, " ", 1);
		i++;
	}
	if (newline)
		write(fd, "\n", 1);
	return (BUILTIN_SUCCESS);
}

int	pwd_c(int fd)
{
	char	*cwd;
	
	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		perror("pwd");
		return (BUILTIN_ERROR);
	}
	write(fd, cwd, ft_strlen(cwd));
	write(fd, "\n", 1);
	free(cwd);
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

	// printf("%d\n", print_exit);
	if (print_exit)
		print("exit\n", 2);
	exit_code = exit_status;
	if (args[1])
	{
		if (args[2])
		{
			print("exit: too many arguments\n", fd);
			return (BUILTIN_ERROR);
		}
		exit_code = atol_s(args[1], &endptr);
		if (*endptr != '\0' || args[1][0] == '\0')
		{
			print("minishell: exit: ", fd);
			print(args[1], fd);
			print(": numeric argument required\n", fd);
			exit(2);
		}
	}
	exit((unsigned char)exit_code);
}


// ➜  minish git:(exec) ✗ ./minishell 
// minishell$ lll
// minishell: lll: command not found
// minishell$ exit
// exit
// ➜  minish git:(exec) ✗ echo $?
// 0 
// it should not be 0 but it should be the last exit status in this case 127
