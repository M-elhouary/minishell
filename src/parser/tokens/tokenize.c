/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houa <mel-houa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 20:03:03 by mel-houa          #+#    #+#             */
/*   Updated: 2025/08/25 07:37:52 by mel-houa         ###   ########.fr       */
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

int	herdoc_token(char **split_words, char *word, t_token_glbst *glbst)
{
	split_words = gc_malloc(glbst->gc, sizeof(char *) * 2);
	if (!split_words)
		return (0);
	split_words[0] = word;
	split_words[1] = NULL;
	return (add_split_words(glbst->tokens, split_words, glbst->gc));
}

static int	process_word_gc(char *line, int *i, t_token_glbst *glbst)
{
	char	**split_words;
	char	*expanded;
	char	*word;
	int		has_vars;
	t_token	*last;

	last = get_last_token(glbst);
	split_words = NULL;
	word = extract_word_gc(line, i, glbst->gc);
	if (!word)
		return (0);
	if (last && last->type == HEREDOC)
		return (herdoc_token(split_words, word, glbst));
	has_vars = has_unquoted_variables(word);
	expanded = expand_variables_gc(word, glbst->env, glbst->cmd, glbst->gc);
	if (has_vars && (!expanded || !*expanded))
		return (handle_empty_expansion(glbst->tokens, glbst->gc));
	split_words = expand_and_split_gc(word, glbst->env, glbst->gc,
			glbst->cmd);
	if (!split_words)
		return (1);
	return (add_split_words(glbst->tokens, split_words, glbst->gc));
}

static int	process_special(char *line, int *i, t_token_glbst *glbst)
{
	if (!skip_spaces(line, i))
		return (0);
	if (handle_special_gc(glbst->tokens, line, i, glbst->gc))
		return (1);
	return (process_word_gc(line, i, glbst));
}

t_token	*tokenize_gc(char *line, t_env *env, t_gc *gc, t_command *cmd)
{
	t_token_glbst	glbst;
	int				i;

	i = 0;
	glbst.tokens = &(t_token *){NULL};
	glbst.env = env;
	glbst.gc = gc;
	glbst.cmd = cmd;
	if (has_unclosed_quote(line))
	{
		print_error("unclosed quote", NULL);
		return (NULL);
	}
	while (line[i])
	{
		if (!process_special(line, &i, &glbst))
			break ;
	}
	return (*(glbst.tokens));
}
