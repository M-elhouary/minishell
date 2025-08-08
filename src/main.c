/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houa <mel-houa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2025/08/08 21:36:35 by mel-houa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"


// static void print_args(char **args) {
// 	int i = 0;
// 	if (args) {
// 		printf("Arguments:\n");
// 		while (args[i]) {
// 			printf("  args[%d]: %s\n", i, args[i]);
// 			i++;
// 		}
// 	} else {
// 		printf("Arguments: (none)\n");
// 	}
// }

// static void print_files(const char *label, char **files) {
// 	int i = 0;
// 	if (files) {
// 		printf("%s:\n", label);
// 		while (files[i]) {
// 			printf("  %s[%d]: '%s'\n", label, i, files[i]);
// 			i++;
// 		}
// 	}
// }

// void print_commands(t_command *cmd)
// {
// 	int cmd_num = 1;
// 	while (cmd)
// 	{
// 		printf("=== Command %d ===\n", cmd_num);
// 		print_args(cmd->args);
// 		print_files("Input files", cmd->infile);
// 		print_files("Output files", cmd->outfile);
// 		printf("Flags:\n");
// 		printf("  append: %d\n", cmd->append);
// 		printf("  heredoc: %d\n", cmd->heredoc);
// 		printf("------------------\n");
// 		cmd = cmd->next;
// 		cmd_num++;
// 	}
// }


void	sigint_(int sig)
{
	(void)sig;
	printf("\n");
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	sigquit_(int sig)
{
	(void)sig;
}
int	main(int ac, char **av, char **env)
{
    char		*line;
    t_token		*tokens;
    t_command	*cmd;
	t_command *new_cmd;
    t_env		*env_list;
    t_gc		gc;
    int			exit_code;

	(void)ac;
	(void)av;
	gc_init(&gc);
	cmd  = malloc(sizeof(t_command));
	if(!cmd)
		return 1;
	env_list = my_env(env);
	//last_exit = 0;
	signal(SIGINT, sigint_);
	signal(SIGQUIT, sigquit_);
	while (1)
	{
		line = readline("minishell$ ");
		if (!line)
			break ;
		if (!*line)
		{
			//free(line);
			continue;
		}
		add_history(line);
		tokens = tokenize_gc(line, env_list, &gc, cmd);
		if (!tokens)
		{
			// Error already printed by print_error, just skip execution
			continue;
		}
		if (check_syntax_token(tokens, cmd))
		{
			cmd = parse_commands(tokens);
			if (cmd)
			{
				cmd->path = locate_cmd(cmd->args[0]);
				// if (!cmd->path)
				exit_code = exec_cmd(cmd, &env_list);
				
				//wher used this variable 
				//last_exit = exit_code;
				if (exit_code != 0)
					printf("[Exit code: %d]\n", exit_code);
				free_commands(cmd);
			}
		}
		free(line);
		gc_free_all(&gc);
	}
	gc_destroy(&gc);
    return (0);
} // Initialize to prevent undefined behavior