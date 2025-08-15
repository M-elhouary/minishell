/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houa <mel-houa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 20:03:03 by mel-houa          #+#    #+#             */
/*   Updated: 2025/08/15 02:34:21 by mel-houa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


// Handle special shell characters
static int	handle_special_gc(t_token **tokens, char *line, int *i, t_gc *gc)
{
	// First check for double-character operators
	// like '>>', '<<' herdoc, and append
	if (handle_double_char_op(tokens, line, i, gc))
		return (1);
	// Then check single-character operators
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
// Add all split words as tokens

/*
 * Process a word from the input line:
 * - Extracts the word (handles quotes)
 * - Expands variables if present
 * - Handles empty expansions (e.g., $UNSET_VAR)
 * - Splits the word if necessary (e.g., after expansion)
 * - Adds the resulting tokens as COMMAND or ARGUMENT
 * Returns 1 on success, 0 on error
 */

int	herdoc_token(char **split_words, char *word, t_token_glbst *glbst)
{
	split_words = gc_malloc(glbst->gc, sizeof(char *) * 2);
	if (!split_words)
	{
		free(word);
		return (0);
	}
	split_words[0] = gc_strdup(glbst->gc, word);
	split_words[1] = NULL;
	free(word);
	return (add_split_words(glbst->tokens, split_words, glbst->gc));
}

static int	process_word_gc(char *line, int *i, t_token_glbst *glbst)
{
	char	**split_words;
	int		has_vars;
	t_token	*last;

	char *expanded, *word;
	// get the last token to check if we are in a heredoc context
	last = get_last_token(glbst);
	split_words = NULL;
	// Extract the word from the line 
	// used for make word  not have quotes and spaces and matchracters
	word = extract_word(line, i);
	if (!word)
		return (0);
	//case special characters >>  dilimiter
	if (last && last->type == HEREDOC)
		return (herdoc_token(split_words, word, glbst));
	// Check for unquoted variables in the word
	//
	has_vars = has_unquoted_variables(word);
	expanded = expand_variables(word, glbst->env, glbst->cmd);
	if (has_vars && (!expanded || !*expanded))
		return (free(word), free(expanded),
			handle_empty_expansion(glbst->tokens, glbst->gc));
	free(expanded);
	split_words = expand_and_split_gc(word, glbst->env, glbst->gc, glbst->cmd);
	free(word);
	if (!split_words)
		return (1);
	return (add_split_words(glbst->tokens, split_words, glbst->gc));
}

// Process special characters in tokenization
static int	process_special(char *line, int *i, t_token_glbst *glbst)
{
	// Skip spaces
	// why skip_spaces is used here? // to ignore leading spaces before processing tokens
	if (!skip_spaces(line, i))
		return (0);
	// Handle special characters
	if (handle_special_gc(glbst->tokens, line, i, glbst->gc))
		return (1);
	// Handle regular word
	return (process_word_gc(line, i, glbst));
}

// Main tokenization function
t_token	*tokenize_gc(char *line, t_env *env, t_gc *gc, t_command *cmd)
{
	// this is global to store tokens, env, gc, and cmd
	t_token_glbst glbst;
	int i;

	i = 0;
	glbst.tokens = &(t_token *){NULL};
	glbst.env = env;
	glbst.gc = gc;
	glbst.cmd = cmd;

	// Check for unclosed quotes
	if (has_unclosed_quote(line))
	{
		print_error("unclosed quote", NULL);
		return (NULL);
	}
	// Process input line
	while (line[i])
	{
		if (!process_special(line, &i, &glbst))
			break ;
	}
	return (*(glbst.tokens));
}