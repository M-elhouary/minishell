/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houa <mel-houa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/20 12:00:00 by mel-houa          #+#    #+#             */
/*   Updated: 2025/08/03 23:23:57 by mel-houa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "parse.h"





static void print_args(char **args) {
	int i = 0;
	if (args) {
		printf("Arguments:\n");
		while (args[i]) {
			printf("  args[%d]: %s\n", i, args[i]);
			i++;
		}
	} else {
		printf("Arguments: (none)\n");
	}
}

static void print_files(const char *label, char **files) {
	int i = 0;
	if (files) {
		printf("%s:\n", label);
		while (files[i]) {
			printf("  %s[%d]: '%s'\n", label, i, files[i]);
			i++;
		}
	}
}

void print_commands(t_command *cmd)
{
	int cmd_num = 1;
	while (cmd)
	{
		printf("=== Command %d ===\n", cmd_num);
		print_args(cmd->args);
		print_files("Input files", cmd->infile);
		print_files("Output files", cmd->outfile);
		printf("Flags:\n");
		printf("  append: %d\n", cmd->append);
		printf("  heredoc: %d\n", cmd->heredoc);
		printf("------------------\n");
		cmd = cmd->next;
		cmd_num++;
	}
}

int	main(int ac, char **av, char **env)
{
	char		*line;
	t_token		*tokens;
	t_command	*cmds;
	t_env		*env_list;
	t_gc		gc;

	(void)ac;
	(void)av;
	gc_init(&gc);
	env_list = my_env(env);
	while (1)
	{
		line = readline("minishell$ ");
		if (!line)
			break ;
		if (*line)
		{
			add_history(line);
			tokens = tokenize_gc(line, env_list, &gc);
			if (tokens)
			{
				if (check_syntax_token(tokens))
				{
					cmds = parse_commands(tokens);
					if (cmds)
					{
						
						print_commands(cmds);
						free_commands(cmds);
					}
				}
			}
		}
		free(line);
		gc_free_all(&gc);
	}
	gc_destroy(&gc);
	return (0);
}