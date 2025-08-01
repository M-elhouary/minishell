/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houa <mel-houa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/20 12:00:00 by mel-houa          #+#    #+#             */
/*   Updated: 2025/08/01 17:45:36 by mel-houa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"



void	print_commands(t_command *cmd)
{
	int	i;
	int	cmd_num;

	cmd_num = 1;
	while (cmd)
	{
		printf("=== Command %d ===\n", cmd_num);
		if (cmd->args)
		{
			printf("Arguments:\n");
			i = 0;
			while (cmd->args[i])
			{
				printf("  args[%d]: '%s'\n", i, cmd->args[i]);
				i++;
			}
		}
		else
			printf("Arguments: (none)\n");
		if (cmd->infile)
		{
			printf("Input files:\n");
			i = 0;
			while (cmd->infile[i])
			{
				printf("  infile[%d]: '%s'\n", i, cmd->infile[i]);
				i++;
			}
		}
		if (cmd->outfile)
		{
			printf("Output files:\n");
			i = 0;
			while (cmd->outfile[i])
			{
				printf("  outfile[%d]: '%s'\n", i, cmd->outfile[i]);
				i++;
			}
		}
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