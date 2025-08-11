/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houa <mel-houa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/20 12:00:00 by mel-houa          #+#    #+#             */
/*   Updated: 2025/08/11 21:54:48 by mel-houa         ###   ########.fr       */
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
	t_command	*tmp_cmd;
    t_env		*env_list;
    t_gc		gc;
    int			exit_code;

	(void)ac;
	(void)av;
	gc_init(&gc);
    cmd = NULL;
	env_list = my_env(env);
	signal(SIGINT, sigint_);
	signal(SIGQUIT, sigquit_);
    
	while (1)
	{
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
            // cmd->infile = NULL;
            // cmd->outfile = NULL;
            cmd->next = NULL;
        }
        
		line = readline("minishell$ ");
		if (!line)
            break;
		if (!*line)
		{
            free(line);
			continue;
		}
		add_history(line);
        
		tokens = tokenize_gc(line, env_list, &gc, cmd);
		if (!tokens)
		{
            free(line);
			continue;
		}
        
        // Check syntax and continue if error (status already set in check_syntax_token)
        if (!check_syntax_token(tokens, cmd))
		{
            free(line);
            gc_free_all(&gc);
            continue;
        }
        handl_herdoc(tokens, env_list, cmd);
		 // Only create new command if syntax is correct
        tmp_cmd = parse_commands(tokens);
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
					tmp_cmd->path = locate_cmd(tmp_cmd->args[0]);
				tmp_cmd = tmp_cmd->next;
			}

            // Set path and execute
            // cmd->path = locate_cmd(cmd->args[0]);
            // exit_code = exec_cmd(cmd, &env_list, 1);
			exit_code = exec_pipeline(cmd, &env_list);
            
            // Update exit status for next command
            cmd->status_exit = exit_code;
            
				if (exit_code != 0)
					printf("[Exit code: %d]\n", exit_code);
		}
        
		free(line);
		gc_free_all(&gc);
	}
    
    // Final cleanup
    	if (cmd)
    		free_commands(cmd);
	gc_destroy(&gc);
    return (0);
} // Initialize to prevent undefined behavior