/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houa <mel-houa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/20 12:00:00 by mel-houa          #+#    #+#             */
/*   Updated: 2025/08/25 23:35:43 by mel-houa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int ac, char **av, char **env)
{
	char *line;
	t_token *tokens;
	t_command *cmd;
	t_command *tmp_cmd;
	t_env *env_list;
	t_gc gc;
	int exit_code;
	int last_exit_status;
	
	(void)ac;
	last_exit_status = 0;
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
			break ;
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
			free(line);
			gc_free_all(&gc);
			continue ;
		}
		if (handl_herdoc_gc(tokens, env_list, cmd, &gc))
			continue ;
		tmp_cmd = parse_commands_gc(tokens, &gc);
		if (tmp_cmd)
		{
			t_command *current = tmp_cmd;
			while (current)
			{
				if (current->args && current->args[0])
				current->path = locate_cmd(current->args[0], env_list);
				current = current->next;
			}
			exit_code = exec_pipeline(tmp_cmd, &env_list);
			cmd->status_exit = exit_code;
		}

		free(line);
		gc_free_all(&gc);
	}
	if (cmd)
		free(cmd);
	gc_destroy(&gc);
	return (0);
}