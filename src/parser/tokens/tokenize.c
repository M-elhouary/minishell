/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houa <mel-houa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 20:03:03 by mel-houa          #+#    #+#             */
/*   Updated: 2025/08/22 11:16:45 by mel-houa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_special_gc(t_token **tokens, char *line, int *i, t_gc *gc)
{
	if (handle_double_char_op(tokens, line, i, gc))
		return (1);
	if (line[*i] == '|')
	{
		create_and_add_token(tokens, "|", PIPE, gc);
		(*i)++;
	}
	else if (line[*i] == '>')
	{
		create_and_add_token(tokens, ">", REDIR_OUT, gc);
		(*i)++;
	}
	else if (line[*i] == '<')
	{
		create_and_add_token(tokens, "<", REDIR_IN, gc);
		(*i)++;
	}
	else
		return (0);
	return (1);
}

int	herdoc_token(char **split_words, char *word, t_token_ctx *ctx)
{
	split_words = gc_malloc(ctx->gc, sizeof(char *) * 2);
	if (!split_words)
	{
		return (0);
	}
	split_words[0] = word;
	split_words[1] = NULL;
	return (add_split_words(ctx->tokens, split_words, ctx->gc));
}

static int	process_word_gc(char *line, int *i, t_token_ctx *ctx)
{
	char	**split_words;
	int		has_vars;
	t_token	*last;

	char *expanded, *word;
	last = get_last_token(ctx);
	split_words = NULL;
	word = extract_word_gc(line, i, ctx->gc);
	if (!word)
		return (0);
	if (last && last->type == HEREDOC)
		return (herdoc_token(split_words, word, ctx));
	has_vars = has_unquoted_variables(word);
	expanded = expand_variables_gc(word, ctx->env, ctx->cmd, ctx->gc);
	if (has_vars && (!expanded || !*expanded))
		return (handle_empty_expansion(ctx->tokens, ctx->gc));
	split_words = expand_and_split_gc_updated(word, ctx->env, ctx->gc,
			ctx->cmd);
	if (!split_words)
		return (1);
	return (add_split_words(ctx->tokens, split_words, ctx->gc));
}

static int	process_special(char *line, int *i, t_token_ctx *ctx)
{
	if (!skip_spaces(line, i))
		return (0);
	if (handle_special_gc(ctx->tokens, line, i, ctx->gc))
		return (1);
	return (process_word_gc(line, i, ctx));
}

t_token	*tokenize_gc(char *line, t_env *env, t_gc *gc, t_command *cmd)
{
	t_token_ctx ctx;
	int i;

	i = 0;
	ctx.tokens = &(t_token *){NULL};
	ctx.env = env;
	ctx.gc = gc;
	ctx.cmd = cmd;
	if (has_unclosed_quote(line))
	{
		print_error("unclosed quote", NULL);
		return (NULL);
	}
	while (line[i])
	{
		if (!process_special(line, &i, &ctx))
			break ;
	}
	return (*(ctx.tokens));
}