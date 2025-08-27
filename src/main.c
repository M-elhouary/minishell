/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-abbo <sel-abbo@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/20 12:00:00 by mel-houa          #+#    #+#             */
/*   Updated: 2025/08/27 07:15:34 by sel-abbo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


typedef struct s_ctx {
    t_command *cmd;
    t_env *env_list;
    t_gc gc;
    int last_exit_status;
    char **env;
} t_ctx;

void setupclean(t_ctx *ctx)
{
    t_clean **cleanchild;
	cleanchild = grepclean();
	(*cleanchild)->env = ctx->env_list;
	(*cleanchild)->cmd = ctx->cmd;
	(*cleanchild)->gc = &ctx->gc;
}

void setup_cmd(t_ctx *ctx)
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

static int handle_syntax(t_token *tokens, t_ctx *ctx)
{
    if (!check_syntax_token(tokens, ctx->cmd))
    {
        ctx->last_exit_status = ctx->cmd->status_exit;
        cleanup_env(ctx->env_list);
        gc_free_all(&ctx->gc);
        ctx->env_list = my_env(ctx->env);
        return 1;
    }
    return 0;
}

static void set_cmd_paths(t_command *tmp_cmd, t_ctx *ctx)
{
    t_command *current = tmp_cmd;
    while (current)
    {
        if (current->args && current->args[0])
            current->path = locate_cmd(current->args[0], ctx->env_list);
		current->status_exit = ctx->last_exit_status;
        current = current->next;
    }
}

static void free_cmd_paths(t_command *tmp_cmd)
{
    t_command *current = tmp_cmd;
    while (current)
    {
        if (current->path)
        {
            free(current->path);
            current->path = NULL;
        }
        current = current->next;
    }
}

int handle_line(char *line, t_ctx *ctx)
{
    t_token *tokens = tokenize_gc(line, ctx->env_list, &ctx->gc, ctx->cmd);
    if (!tokens)
    {
        gc_free_all(&ctx->gc);
        return 1;
    }
    if (handle_syntax(tokens, ctx))
    {
        return 1;
    }
    if (handl_herdoc_gc(tokens, ctx->env_list, ctx->cmd, &ctx->gc))
    {
        ctx->last_exit_status = ctx->cmd->status_exit;
        gc_free_all(&ctx->gc);
        return 1;
    }
    t_command *tmp_cmd = parse_commands_gc(tokens, &ctx->gc);
    if (tmp_cmd)
    {
        set_cmd_paths(tmp_cmd, ctx);
        int exit_code = exec_pipe(tmp_cmd, &ctx->env_list);
        ctx->cmd->status_exit = exit_code;
        ctx->last_exit_status = exit_code;
        free_cmd_paths(tmp_cmd);
    }
    gc_free_all(&ctx->gc);
    return 0;
}



static int process_input_line(t_ctx *ctx, char *line)
{
    if (!line)
    {
        if (isatty(STDIN_FILENO))
            write(STDERR_FILENO, "exit\n", 5);
        return 1;
    }
    if (!*line)
    {
        free(line);
        return 0;
    }
    add_history(line);
    if (handle_line(line, ctx))
    {
        free(line);
        return 0;
    }
    free(line);
    return 0;
}

void run_shell_loop(t_ctx *ctx)
{
    char *line;
    while (1)
    {
        signal(SIGINT, sigint_interactive);
        signal(SIGQUIT, sigquit_interactive);
        setup_cmd(ctx);
        setupclean(ctx);
        line = readline("minishell$ ");
        if (process_input_line(ctx, line))
            break;
    }
}




int main(int ac, char **av, char **env)
{
    t_ctx ctx;
    ctx.cmd = NULL;
    ctx.env = env;
    ctx.last_exit_status = 0;
    (void)ac;
    (void)av;
    gc_init(&ctx.gc);
    ctx.env_list = my_env(env);
    run_shell_loop(&ctx);
    if (ctx.cmd)
        free(ctx.cmd);
    cleanup_env(ctx.env_list);
    gc_destroy(&ctx.gc);
    return (ctx.last_exit_status);
}