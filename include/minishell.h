/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: houardi <houardi@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 18:50:17 by houardi           #+#    #+#             */
/*   Updated: 2025/08/04 04:07:08 by houardi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef MINISHELL_H
#define MINISHELL_H

#include <sys/wait.h>
#include <errno.h>
#include <signal.h>
#include <linux/limits.h>
#include <sys/stat.h>
#include <parse.h>

typedef enum
{
	NOT_BUILTIN = -1,
	BUILTIN_SUCCESS = 0,
	BUILTIN_ERROR = 1
}	t_builtin;

char		*locate_cmd(char *cmd);
t_command	*create_cmd(char **args);
int			exec_cmd(t_command *cmd, t_env **env);
void		free_cmd(t_command *cmd);
long		atol_s(const char *str, char **endptr);

int	echo_c(char **args);
int	pwd_c();
int	cd_c(char **args, t_env **env);
int	env_c(t_env *env);
int	export_c(char **args, t_env **env);
int	unset_c(char **args, t_env **env);
int	exit_c(char **args);
t_builtin	exec_builtin(t_command *cmd, t_env **env);

char	*ft_strcat(char *dst, char *src);
int		set_env_value(t_env **env, char *key, char *value);
int		unset_env_value(t_env **env, char *key);
char	**env_to_array(t_env *env);
void	free_env_array(char **envp);

#endif