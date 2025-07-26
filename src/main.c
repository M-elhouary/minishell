/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houa <mel-houa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/20 12:00:00 by mel-houa          #+#    #+#             */
/*   Updated: 2025/07/26 20:55:15 by mel-houa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_tokens(t_token *token)
{
	const char	*types[] = {"WORD", "PIPE", "REDIR_IN", "REDIR_OUT", 
		"REDIR_APPEND", "HEREDOC", "ARGUMENT"};
	const char	*separator = "----------------------";

	printf("\n%s\nTOKEN LIST:\n%s\n", separator, separator);
	while (token)
	{
		if (token->value)
		{
			printf("Content: %-15s\n", token->value);
			printf("Type:   %-15s\n", types[token->type]);
			printf("%s\n", separator);
		}
		token = token->next;
	}
	printf("\n");
}

int	main(int ac, char **av, char **env)
{
	char	*line;
	t_token	*tokens;
	t_env	*env_list;

	(void)ac;
	(void)av;
	t_exit *exit = malloc(sizeof(t_exit));
	if (!exit) 
    	return (perror("malloc failed"), 1); 

	t_flags *flags = malloc(sizeof(t_flags));
	if (!flags) 
    	return (perror("malloc failed"), 1);
	flags->f_squote = 0;
	env_list = my_env(env);
	exit->exit_status = 0;
	while (1)
	{
		line = readline("minishell$ ");
		if (!line)
			break ;
		if (*line)
		{
			add_history(line);
			tokens = tokenize(line, env_list, flags);
			if (tokens)
			{	
				if (check_syntax_token(tokens))
				{
					
					print_tokens(tokens);  
				}
				else
					exit->exit_status = 2;
					
				free_token(tokens);
			}
		}
		free(line);
	}
	return (0);
}