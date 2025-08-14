/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   herdoc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houa <mel-houa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 15:18:28 by mel-houa          #+#    #+#             */
/*   Updated: 2025/08/14 02:42:50 by mel-houa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	process_herdoc_line(char *line, int fd, t_env *env_list,
		t_command *cmd)
{
	char	*expanded;

	expanded = expand_var_in_string(line, env_list, cmd);
	free(line);
	write(fd, expanded, ft_strlen(expanded));
	write(fd, "\n", 1);
	free(expanded);
}

static int	prepare_delimiter(char **clean_delimiter, char *delimiter,
		int *quotes_flag)
{
	*quotes_flag = has_quotes(delimiter);
	*clean_delimiter = remove_quotes(ft_strdup(delimiter));
	if (!*clean_delimiter)
		return (0);
	return (1);
}

void	similation_herdoc(char *delimiter, int fd, t_env *env_list,
        t_command *cmd)
{
    int quotes_for_expansion;
    char *line, *clean_delimiter;
    
    /* Save current execution state */
    int old_exec_state = get_execution_state();
    
    if (!prepare_delimiter(&clean_delimiter, delimiter, &quotes_for_expansion))
    {
        close(fd);
        free(delimiter);
        return;
    }
    
    /* Setup special signal handling for heredoc */
    signal(SIGINT, SIG_DFL);   /* Default SIGINT for heredoc */
    signal(SIGQUIT, SIG_IGN);  /* Ignore SIGQUIT in heredoc */
    
    while (1)
    {
        line = readline(">");
        if (!line || ft_strcmp(line, clean_delimiter) == 0)
        {
            free(line);
            break ;
        }
        if (!quotes_for_expansion)
            process_herdoc_line(line, fd, env_list, cmd);
        else
        {
            write(fd, line, ft_strlen(line));
            write(fd, "\n", 1);
            free(line);
        }
    }
    /* Restore previous signal handlers */
    set_execution_state(old_exec_state);
    if (get_execution_state() == 0)
    {
        signal(SIGINT, sigint_handler);
        signal(SIGQUIT, sigquit_handler);
    }
    
    free_and_close(clean_delimiter, fd, delimiter);
}

/* Process a heredoc token */
static int	process_heredoc_token(t_token *tmp, t_env *env_list, t_command *cmd,
		int index)
{
	char	*file_name;
	pid_t	pid;

	int wait_result, status, fd;
	file_name = gen_file_name(index, tmp->value);
	fd = open(file_name, O_WRONLY | O_CREAT | O_TRUNC, 0600);
	if (fd < 0)
		return (free(file_name), 0);
	pid = fork();
	if (pid < 0)
		return (close(fd), free(file_name), 0);
	if (pid == 0)
	{
		similation_herdoc(ft_strdup(tmp->next->value), fd, env_list, cmd);
		exit(0);
	}
	close(fd); // Close fd in parent process immediately
	wait_result = waitpid(pid, &status, 0);
	if (wait_result == -1)
		return (free(file_name), 0);
	if (WIFEXITED(status) && WEXITSTATUS(status) == 0)
		return (tmp->next->value = ft_strdup(file_name), free(file_name), 1);
	else if (WIFSIGNALED(status))
		return (free(file_name), 0);
	return (free(file_name), 0);
}

/* Wait for heredoc child process */
int	wait_for_child(pid_t pid, t_token *tmp, char *file_name, int fd)
{
	int	status;
	int	wait_result;

	close(fd);
	wait_result = waitpid(pid, &status, 0);
	if (wait_result == -1)
	{
		free(file_name);
		return (0);
	}
	if (WIFEXITED(status) && WEXITSTATUS(status) == 0)
	{
		tmp->next->value = ft_strdup(file_name);
		free(file_name);
		return (1);
	}
	free(file_name);
	return (0);
}

/* Main heredoc handler function */
void	handl_herdoc(t_token *token, t_env *env_list, t_command *cmd)
{
	t_token	*tmp;
	int		index;

	tmp = token;
	if (!tmp)
		return ;
	index = 5;
	while (tmp)
	{
		if (tmp->type == HEREDOC && tmp->next && tmp->next->type == ARGUMENT
			&& !tmp->next->is_empty_expansion)
		{
			if (!process_heredoc_token(tmp, env_list, cmd, index))
				return ; // Stop if heredoc processing failed or was interrupted
			index = (index == 62) ? 5 : index + 1;
		}
		if (tmp)
			tmp = tmp->next;
	}
}

// void handl_herdoc(t_token *token, t_env *env_list, t_command *cmd)
// {
//     t_token *tmp;
//     pid_t pid;
//     int wait_result;
//     int status;
//     char *file_name;
//     int fd;
//     static int index;

//     tmp = token;
//     if (!tmp)
//      return ;
//     index = 5;
//     while (tmp)
//     {
//         if (tmp->type == HEREDOC && tmp->next && tmp->next->type == ARGUMENT)
//         {
//             if (tmp->next->is_empty_expansion)
//             {
//                 tmp = tmp->next;
//                 if (tmp)
//                     tmp = tmp->next;
//                 continue ;
//             }
//             file_name = gen_file_name(index, tmp->value);
//             fd = open(file_name, O_WRONLY | O_CREAT, 0600);
//             if (fd < 0)
//             {
//                 free(file_name);
//                 return ;
//             }
//             pid = fork();
//             if (pid < 0)
//                 return ;

//             if (pid == 0)
//             {
//                 similation_herdoc(ft_strdup(tmp->next->value), fd, env_list,
//                 exit(0);
//             }
//             index++;
//             if(index == 63)
//                 index = 5;
//             wait_result = waitpid(pid, &status, 0);
//             if (wait_result == -1)
//             {
//                  perror("waitpid failed");
//                 return ;
//             }
//             if (WIFEXITED(status) && WEXITSTATUS(status) == 0)
//             {
//                 tmp->next->value = ft_strdup(file_name);
//                 free(file_name);
//             }
//             else if (WIFSIGNALED(status))
//                 return ;
//         }
//         if (tmp)
//             tmp = tmp->next;
//     }
// }
