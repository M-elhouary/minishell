/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: houardi <houardi@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 18:50:17 by houardi           #+#    #+#             */
/*   Updated: 2025/08/26 10:07:27 by houardi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef MINISHELL_H
#define MINISHELL_H

#include <sys/wait.h>
#include <errno.h>
#include <signal.h>
#include <linux/limits.h>
#include <sys/stat.h>
#include <limits.h>
#include "parse.h"

typedef enum
{
	NOT_BUILTIN = -1,
	BUILTIN_SUCCESS = 0,
	BUILTIN_ERROR = 1
}	t_builtin;

typedef struct s_export
{
	char			*var;
	struct s_export	*next;
}	t_export;

typedef struct s_pipe_state
{
	int	prev_read_fd;
	int	current_pipe[2];
	int	cmd_index;
	int	total_cmds;
	int	pipe_created;
}	t_pipe_state;

typedef struct s_exec_context
{
	int	fd;
	int	in_child;
	int	original_stdin;
	int	original_stdout;
}	t_exec_var;

typedef struct s_pipe_var
{
	t_command			*cmd_list;
	t_env				**env;
	int					cmd_count;
	pid_t				*pids;
	int					procs_created;
	int					exit_status;
	struct sigaction	old_int;
	struct sigaction	old_quit;
}	t_pipe_var;

char		*locate_cmd(char *cmd, t_env *env);
char		**split_path(char *path_env);
char    	*full_path(char **paths, char *cmd);
void	free_paths(char **paths);

int			exec_cmd(t_command *cmd, t_env **env, int fd, int in_child);
void		exec_child(t_command *cmd, t_env **env);
int			wait_child(pid_t pid);
void		free_cmd(t_command *cmd);
long		atol_s(char *str, char **endptr);

int	echo_c(char **args, int fd);
int	pwd_c(t_env *env);
int	cd_c(char **args, t_env **env, int fd);
int	env_c(t_env *env, int fd);
int	export_c(char **args, t_env **env, int fd);
int	unset_c(char **args, t_env **env);
int	exit_c(char **args, int fd, int exit_status, int print_exit);
t_builtin	exec_builtin(t_command *cmd, t_env **env, int fd);

char	*ft_strcat(char *dst, char *src);
int		set_env_value(t_env **env, char *key, char *value);
int		set_env_exported_only(t_env **env, char *key);
int		unset_env_value(t_env **env, char *key);
char	**env_to_array(t_env *env);
void	free_env_array(char **envp);
void	print(char *s, int fd);
int		exec_pipe(t_command *cmd_list, t_env **env);
int		exit_status(int status);

int		handle_redirections(t_redirection *redirections);
int		process_single_redirection(t_redirection *redir);

/* Execution state management */
int get_execution_state(void);
void set_execution_state(int state);

/* Signal handling functions */
void sigint_handler(int sig);
void sigquit_handler(int sig);

int	pipe_loop(t_command *cmd_list, t_env **env, pid_t *pids, int cmd_count);
int	wait_children(pid_t *pids, int cmd_count);
int	open_next_pipe(int is_last, int curr_pipe[2]);
int	count_cmds(t_command *cmd);

int	handle_external_command_mode(t_command *cmd, t_env **env, int fd, int in_child);
int	validate_cmd_path(char *path, char *cmd);
int	check_cmd_path(char *path, char *cmd);

int	process_cmd(t_command *cmd, t_env **env, t_exec_var *ev);

void	restore_file_descriptors(int original_stdin, int original_stdout);

/* Export list manipulation functions */
t_export	*create_export_node(char *var);
void		add_export_node(t_export **head, t_export *new_node);
t_export	*find_export_node(t_export *head, char *var);
void		remove_export_node(t_export **head, char *var);
void		free_export_list(t_export *head);
int			export_list_size(t_export *head);
int			export_empty_var(t_export **export_list, char *var);

/* Single global variable to track last observed signal (0 if none) */
extern volatile sig_atomic_t g_last_signal;

#endif