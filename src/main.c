/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: houardi <houardi@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/20 12:00:00 by mel-houa          #+#    #+#             */
/*   Updated: 2025/08/26 10:52:55 by houardi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


// sudo
// last_status on exit cmd
// ls on unset path

int	main(int ac, char **av, char **env)
{
	char *line;
	t_token *tokens;
	t_command *cmd;
	t_command *tmp_cmd;
	t_env *env_list;
	t_gc gc;
	int exit_code;
	int last_exit_status = 0;
	int	prev_cmd;

	(void)ac;
	(void)av;
	gc_init(&gc);
	cmd = NULL;
	env_list = my_env(env);

	while (1)
	{
		signal(SIGINT, sigint_interactive);
		signal(SIGQUIT, sigquit_interactive);
		if (!cmd)
		{
			cmd = malloc(sizeof(t_command));
			if (!cmd)
				return (1);
			cmd->status_exit = last_exit_status;
			cmd->args = NULL;
			cmd->path = NULL;
			cmd->redirections = NULL;
			cmd->print_exit = 0;
			cmd->next = NULL;
		}

		line = readline("minishell$ ");
		if (!line)
		{
			if (isatty(STDIN_FILENO))
				write(STDERR_FILENO, "exit\n", 5);
			break ;
		}
		if (!*line)
		{
			free(line);
			continue ;
		}
		add_history(line);
		tokens = tokenize_gc(line, env_list, &gc, cmd);
		if (!tokens)
		{
			free(line);
			continue ;
		}
		if (!check_syntax_token(tokens, cmd))
		{
			last_exit_status = cmd->status_exit;
			free(line);
			gc_free_all(&gc);
			continue ;
		}
		if (handl_herdoc_gc(tokens, env_list, cmd, &gc))
			continue ;
		tmp_cmd = parse_commands_gc(tokens, &gc);
		if (tmp_cmd)
		{
			prev_cmd = cmd->status_exit;
			t_command *current = tmp_cmd;
			while (current)
			{
				if (current->args && current->args[0])
					current->path = locate_cmd(current->args[0], env_list);
				current = current->next;
			}

			exit_code = exec_pipe(tmp_cmd, &env_list);
			cmd->status_exit = exit_code;
			last_exit_status = exit_code;
		}

		free(line);
		gc_free_all(&gc);
	}
	if (cmd)
		free(cmd);
	gc_destroy(&gc);
	return (last_exit_status);
}