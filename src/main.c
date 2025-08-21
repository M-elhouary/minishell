/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houa <mel-houa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/20 12:00:00 by mel-houa          #+#    #+#             */
/*   Updated: 2025/08/21 16:59:16 by mel-houa         ###   ########.fr       */
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
	int last_exit_status = 0;  // Persistent exit status storage

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
			cmd = malloc(sizeof(t_command));  // Use malloc - don't let GC manage it
			if (!cmd)
				return (1);
			cmd->status_exit = last_exit_status;  // Restore previous exit status
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
		if(handl_herdoc_gc(tokens, env_list, cmd, &gc))
			continue;
		// Only create new command if syntax is correct
		tmp_cmd = parse_commands_gc(tokens, &gc);
		// printf("%d\n", cmd->status_exit);
		if (tmp_cmd)
		{
			// Set paths for the new command
			t_command *current = tmp_cmd;
			while (current)
			{
				if (current->args && current->args[0])
					current->path = locate_cmd(current->args[0], env_list);
				current = current->next;
			}

			// Execute using the new command from parse_commands_gc
			exit_code = exec_pipeline(tmp_cmd, &env_list);
			// Update exit status in persistent cmd for next command
			cmd->status_exit = exit_code;
			last_exit_status = exit_code;  // Save exit status persistently
			// printf("%d\n", cmd->status_exit);
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
	// Free the persistent cmd object
	if (cmd)
		free(cmd);
	gc_destroy(&gc);
	return (0);
}