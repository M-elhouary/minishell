/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houa <mel-houa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/20 12:00:00 by mel-houa          #+#    #+#             */
/*   Updated: 2025/07/31 18:59:10 by mel-houa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"



void print_commands(t_command *cmd)
{
    int i;
    while (cmd)
    {
        printf("Command:\n");
        for (i = 0; cmd->args && cmd->args[i]; i++)
            printf("  arg[%d]: %s\n", i, cmd->args[i]);
        if (cmd->infile)
            printf("  infile: %s\n", cmd->infile);
        if (cmd->outfile)
            printf("  outfile: %s (append: %d)\n", cmd->outfile, cmd->append);
        if (cmd->heredoc)
            printf("  heredoc: yes\n");
        printf("------------------\n");
        cmd = cmd->next;
    }
}


int main(int ac, char **av, char **env)
{
    (void)ac;
    (void)av;
    char *line;
    t_token *tokens;
    t_env *env_list = my_env(env);

    while (1)
    {
        line = readline("minishell$ ");
        if (!line)
            break;
        if (*line)
        {
            add_history(line);
            tokens = tokenize(line, env_list);
            if (tokens)
            {
                if (check_syntax_token(tokens))
                {
                    t_command *cmds = parse_commands(tokens);
                    print_commands(cmds);
                    // free commands here if you have free function
                }
                free_token(tokens);
            }
        }
        free(line);
    }
    return 0;
}