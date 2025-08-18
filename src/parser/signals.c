/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houa <mel-houa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 02:19:33 by mel-houa          #+#    #+#             */
/*   Updated: 2025/08/18 19:05:00 by mel-houa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//minishell$ ^C
//minishell$ echo $?
//0 ==> 130


// minishell$ cat  | cat 
// 130
// 130
// ^C
// minishell$ minishell$ [Exit code: 130] one prompt 


// minishell$ cat | rev
// 130
// 130
// ^C
// minishell$ 
// minishell$ minishell$ 


int g_in_execution;

// g_in_execution = 0;

int get_execution_state(void)
{
    return g_in_execution;
}

void set_execution_state(int state)
{
    g_in_execution = state;
}

void	sigint_handler(int sig)
{
    (void)sig;
    
    if (get_execution_state() == 0)
    {
        /* Interactive mode (shell prompt) */
        write(STDOUT_FILENO, "\n", 1);
        rl_on_new_line();
        rl_replace_line("", 0);
        rl_redisplay();
    }
    else
    {
        /* Command execution mode */
        write(STDOUT_FILENO, "\n", 1);
    }
}

void	sigquit_handler(int sig)
{
    (void)sig;
    
    if (g_in_execution == 0)
    {
        /* Interactive mode (shell prompt) - do nothing */
        rl_redisplay();
    }
    else
    {
        /* Command execution mode */
        write(STDOUT_FILENO, "Quit (core dumped)\n", 19);
    }
}