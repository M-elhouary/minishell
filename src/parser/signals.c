/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: houardi <houardi@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 02:19:33 by mel-houa          #+#    #+#             */
/*   Updated: 2025/08/20 02:53:24 by houardi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	sigint_child_handler(int sig)
{
    (void)sig;
    write(STDOUT_FILENO, "\n", 1);
    exit(130);
}


void	sigint_interactive(int sig)
{
    (void)sig;
    write(STDOUT_FILENO, "\n", 1);
    rl_on_new_line();
    rl_replace_line("", 0);
    rl_redisplay();
}

void	sigquit_interactive(int sig)
{
    (void)sig;
    rl_on_new_line();
    rl_redisplay();
}