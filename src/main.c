/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houa <mel-houa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/20 12:00:00 by mel-houa          #+#    #+#             */
/*   Updated: 2025/08/19 05:11:35 by mel-houa         ###   ########.fr       */
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

	(void)ac;
	(void)av;
	// Initialize garbage collector
	gc_init(&gc);
	cmd = NULL;
	env_list = my_env(env);
	

	while (1)
	{
		signal(SIGINT, sigint_interactive);
        signal(SIGQUIT, sigquit_interactive);
		// signal(SIGINT, sigint_handler);
		// signal(SIGQUIT, SIG_IGN); // Ignore SIGQUIT in main loop
		// Ensure cmd always exists for $? expansion
		if (!cmd)
		{
			cmd = malloc(sizeof(t_command));
			if (!cmd)
				return (1);
			cmd->status_exit = 0;
			cmd->args = NULL;
			cmd->path = NULL;
			cmd->redirections = NULL;
			cmd->print_exit = 0;
			cmd->next = NULL;
	
		}

		line = readline("minishell$ "); // how readline works
		if (!line)
			break ;
		if (!*line)
		{
			free(line);
			continue ;
		}
		add_history(line); // why and how work this function
		tokens = tokenize_gc(line, env_list, &gc, cmd);
		if (!tokens)
		{
			free(line);                               
			continue ;
		}

		// Check syntax and continue if error (status already set in check_syntax_token)
		if (!check_syntax_token(tokens, cmd))
		{
			free(line);
			gc_free_all(&gc);
			continue ;
		}
		if(handl_herdoc(tokens, env_list, cmd))
			continue;
		// Only create new command if syntax is correct
		tmp_cmd = parse_commands(tokens);
		//  printf("%d\n", cmd->status_exit);
		if (tmp_cmd)
		{
			// Save the previous exit status in the new command
			int prev_status = cmd->status_exit;

			// Free old cmd safely
			free_commands(cmd);

			// Use new command and restore exit status
			cmd = tmp_cmd;
			cmd->status_exit = prev_status;

			tmp_cmd = cmd;
			while (tmp_cmd)
			{
				if (tmp_cmd->args && tmp_cmd->args[0])
					tmp_cmd->path = locate_cmd(tmp_cmd->args[0], env_list);
				tmp_cmd = tmp_cmd->next;
			}

			// Set path and execute
			// cmd->path = locate_cmd(cmd->args[0]);
			// exit_code = exec_cmd(cmd, &env_list, 1);
			exit_code = exec_pipeline(cmd, &env_list);

			// Update exit status for next command
			cmd->status_exit = exit_code;
		//  printf("exec  %d\n", cmd->status_exit);

			// if (exit_code != 0)
			// 	printf("[Exit code: %d]\n", exit_code);
			// if (exit_code != 0)
			// 	printf("[Exit code: %d]\n", exit_code);
		}

		free(line);
		gc_free_all(&gc);
	}

	// Final cleanup
	if (cmd)
		free_commands(cmd);
	gc_destroy(&gc);
	return (0);
}