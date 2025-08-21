/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   herdoc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houa <mel-houa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 15:18:28 by mel-houa          #+#    #+#             */
/*   Updated: 2025/08/21 23:41:12 by mel-houa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"



// unset: `-TEST': not a valid identifier
// minishell$ unset
// unset: not enough arguments
// minishell$ unset lll
// minishell$ unset 55L
// unset: `55L': not a valid identifier

typedef struct s_heredoc_ctx
{
	char		*delimiter;
	int			fd;
	t_env		*env_list;
	t_command	*cmd;
	t_gc		*gc;
}	t_heredoc_ctx;


static void	process_herdoc_line(char *line, int fd, t_env *env_list,
        t_command *cmd, t_gc *gc)
{
    char	*expanded;

    expanded = expand_var_in_string_gc(line, env_list, cmd, gc);
    free(line);
    write(fd, expanded, ft_strlen(expanded));
    write(fd, "\n", 1);
}

void	similation_herdoc(char *delimiter, int fd, t_env *env_list,
        t_command *cmd, t_gc *gc)
{
    int quotes_for_expansion;
    char *line, *clean_delimiter;
    signal(SIGINT, sigint_child_handler);
    if (!prepare_delimiter_gc(&clean_delimiter, delimiter, &quotes_for_expansion, gc))
    {
        close(fd);
        free(delimiter);
        return;
    }
    while (1)
    {
        line = readline(">");
        if(!line)
        {
            //free(line);
            print_error(delimiter, "warning: here-document at line 1 delimited by end-of-file wanted ==> ");
            break ;
        }
        if (ft_strcmp(line, clean_delimiter) == 0)
        {
            free(line);
            break ;
        }
        if (!quotes_for_expansion)
            process_herdoc_line(line, fd, env_list, cmd, gc);
        else
        {
            write(fd, line, ft_strlen(line));
            write(fd, "\n", 1);
            free(line);
        }
    }
    free_and_close_gc(clean_delimiter, fd, delimiter, gc);
}

static int	process_heredoc_token(t_token *tmp, t_env *env_list, t_command *cmd, t_gc *gc)
{
    char	*file_name;
    static int random_nb;
    int	pid = 0;

    int wait_result, status, fd;
    signal(SIGINT, SIG_IGN);
    signal(SIGQUIT, SIG_IGN);
    pid = fork();
    if (pid < 0)
        return (0);
    if (pid != 0)
    {
        random_nb = pid;
        file_name = gen_file_name_gc(tmp->next->value, random_nb, gc);
        fd = open(file_name, O_WRONLY | O_CREAT | O_TRUNC, 0600);
        if (fd < 0)
            return (0);
    }
    if (pid == 0)
        exit (0);
    pid = fork();
    if (pid == 0)
    {
        similation_herdoc(gc_strdup(gc, tmp->next->value), fd, env_list, cmd, gc);
        close(fd);
        exit(0);
    }
    wait_result = waitpid(pid, &status, 0);
    close(fd); 
    if (wait_result == -1)
        return (0);
    if (WIFEXITED(status))
    {
        cmd->status_exit = WEXITSTATUS(status);
        return (tmp->next->value = gc_strdup(gc, file_name), cmd->status_exit);
    }
    return (0);
}

int	handl_herdoc_gc(t_token *token, t_env *env_list, t_command *cmd, t_gc *gc)
{
    t_token	*tmp;

    tmp = token;
    if (!tmp)
        return 1;
    while (tmp)
    {
        if (tmp->type == HEREDOC && tmp->next && tmp->next->type == ARGUMENT
            && !tmp->next->is_empty_expansion)
        {
            if (process_heredoc_token(tmp, env_list, cmd, gc))
                return (1) ; // Stop if heredoc processing failed or was interrupted
        }
        if (tmp)
            tmp = tmp->next;
    }
    return 0;
}
