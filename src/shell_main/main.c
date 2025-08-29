/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houa <mel-houa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/20 12:00:00 by mel-houa          #+#    #+#             */
/*   Updated: 2025/08/27 22:16:06 by mel-houa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_last_signal;

static int	handle_heredoc_and_exec(t_token *tokens, t_ctx *ctx)
{
	t_command	*tmp_cmd;
	int			exit_code;

	if (handl_herdoc_gc(tokens, ctx->env_list, ctx->cmd, &ctx->gc))
		return (ctx->last_exit_status = ctx->cmd->status_exit,
			gc_free_all(&ctx->gc), 1);
	tmp_cmd = parse_commands_gc(tokens, &ctx->gc);
	if (tmp_cmd)
	{
		set_cmd_paths(tmp_cmd, ctx);
		exit_code = exec_pipe(tmp_cmd, &ctx->env_list);
		if (g_last_signal == SIGINT)
			exit_code = 130;
		else if (g_last_signal == SIGQUIT)
			exit_code = 131;
		ctx->cmd->status_exit = exit_code;
		ctx->last_exit_status = exit_code;
	}
	return (gc_free_all(&ctx->gc), 0);
}

int	handle_line(char *line, t_ctx *ctx)
{
	t_token	*tokens;

	tokens = tokenize_gc(line, ctx->env_list, &ctx->gc, ctx->cmd);
	if (!tokens)
		return (gc_free_all(&ctx->gc), 1);
	if (handle_syntax(tokens, ctx))
		return (1);
	return (handle_heredoc_and_exec(tokens, ctx));
}

static int	process_input_line(t_ctx *ctx, char *line)
{
	if (!line)
	{
		if (isatty(STDIN_FILENO))
			write(STDERR_FILENO, "exit\n", 5);
		return (1);
	}
	if (!*line)
	{
		free(line);
		return (0);
	}
	add_history(line);
	if (handle_line(line, ctx))
	{
		free(line);
		return (0);
	}
	free(line);
	return (0);
}

void	run_shell_loop(t_ctx *ctx)
{
	char	*line;

	while (1)
	{
		signal(SIGINT, sigint_interactive);
		signal(SIGQUIT, SIG_IGN);
		setup_cmd(ctx);
		setupclean(ctx);
		line = readline("minishell$ ");
		if (g_last_signal == SIGINT)
		{
			ctx->last_exit_status = 130;
			ctx->cmd->status_exit = 130;
			g_last_signal = 0;
		}
		if (process_input_line(ctx, line))
			break ;
	}
}

int	main(int ac, char **av, char **env)
{
	t_ctx	ctx;

	ctx.cmd = NULL;
	ctx.env = env;
	ctx.last_exit_status = 0;
	(void)ac;
	(void)av;
	gc_init(&ctx.gc);
	ctx.env_list = my_env(env);
	run_shell_loop(&ctx);
	clean_all();
	return (ctx.last_exit_status);
}
