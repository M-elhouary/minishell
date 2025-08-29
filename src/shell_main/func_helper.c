/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   func_helper.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houa <mel-houa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 10:56:02 by mel-houa          #+#    #+#             */
/*   Updated: 2025/08/27 23:02:32 by mel-houa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	setupclean(t_ctx *ctx)
{
	t_clean	**cleanchild;

	cleanchild = grepclean();
	(*cleanchild)->env = ctx->env_list;
	(*cleanchild)->cmd = ctx->cmd;
	(*cleanchild)->gc = &ctx->gc;
}

void	setup_cmd(t_ctx *ctx)
{
	if (!ctx->cmd)
	{
		ctx->cmd = malloc(sizeof(t_command));
		if (!ctx->cmd)
			exit(1);
	}
	ctx->cmd->status_exit = ctx->last_exit_status;
	ctx->cmd->args = NULL;
	ctx->cmd->path = NULL;
	ctx->cmd->redirections = NULL;
	ctx->cmd->print_exit = 0;
	ctx->cmd->next = NULL;
}

int	handle_syntax(t_token *tokens, t_ctx *ctx)
{
	if (!check_syntax_token(tokens, ctx->cmd))
	{
		ctx->last_exit_status = ctx->cmd->status_exit;
		cleanup_env(ctx->env_list);
		gc_free_all(&ctx->gc);
		ctx->env_list = my_env(ctx->env);
		return (1);
	}
	return (0);
}

void	set_cmd_paths(t_command *tmp_cmd, t_ctx *ctx)
{
	t_command	*current;

	current = tmp_cmd;
	while (current)
	{
		if (current->args && current->args[0])
			current->path = locate_cmd(current->args[0], ctx->env_list);
		current->status_exit = ctx->last_exit_status;
		current = current->next;
	}
}
