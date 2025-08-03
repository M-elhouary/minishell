/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: houardi <houardi@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 18:48:24 by houardi           #+#    #+#             */
/*   Updated: 2025/08/04 00:05:05 by houardi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// void	sigint_(int sig)
// {
// 	(void)sig;
// 	printf("\n");
// 	rl_on_new_line();
// 	rl_replace_line("", 0);
// 	rl_redisplay();
// }

// void	sigquit_(int sig)
// {
// 	(void)sig;
// }

// int main(int ac, char **av, char **envp)
// {
// 	t_command	*cmd;
// 	char		*input;
// 	char		**args;
// 	int			exit_code;
// 	int			last_exit;
// 	t_env		*env;
	
// 	(void)ac;
// 	(void)av;
// 	env = my_env(envp);
// 	last_exit = 0;
// 	signal(SIGINT, sigint_);
// 	signal(SIGQUIT, sigquit_);
// 	while (1)
// 	{
// 		input = readline("minishell$ ");
// 		if (!input)
// 		{
// 			printf("exit\n");
// 			break;
// 		}
// 		if (!*input)
// 		{
// 			free(input);
// 			continue;
// 		}
// 		add_history(input);
// 		args = parse_input(input);
// 		if (!args)
// 		{
// 			free(input);
// 			continue;
// 		}
// 		cmd = create_cmd(args);
// 		if (!cmd)
// 		{
// 			free_args(args);
// 			free(input);
// 			continue;
// 		}
// 		exit_code = exec_cmd(cmd, &env);
// 		last_exit = exit_code;
// 		if (exit_code != 0)
// 			printf("[Exit code: %d]\n", exit_code);
// 		free_cmd(cmd);
// 		free_args(args);
// 		free(input);
// 	}
// 	rl_clear_history();
// 	free_env(env);
// 	return (last_exit);
// }
