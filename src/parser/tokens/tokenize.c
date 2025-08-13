/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houa <mel-houa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 20:03:03 by mel-houa          #+#    #+#             */
/*   Updated: 2025/08/13 18:07:17 by mel-houa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


// Include the main minishell header for required types and function declarations
#include "minishell.h"




// Create a new token, allocating memory using the garbage collector
static t_token	*create_token_gc(char *value, t_token_type type, t_gc *gc)
{
	t_token	*new;

	new = gc_malloc(gc, sizeof(t_token));
	if (!new)
		return (NULL);
	new->value = value;
	new->type = type;
	new->is_empty_expansion = 0;
	new->next = NULL;
	return (new);
}


// Add a token to the end of the token linked list
static void	add_token(t_token **head, t_token *new)
{
	t_token	*tmp;

	if (!new)
		return ;
	if (!*head)
		*head = new;
	else
	{
		tmp = *head;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
	}
}




// Handle special shell characters (|, >, >>, <, <<) and add them as tokens
// Returns 1 if a special token was found and handled, 0 otherwise
static int	handle_special_gc(t_token **tokens, char *line, int *i, t_gc *gc)
{
	if (line[*i] == '|') {
		add_token(tokens, create_token_gc(gc_strdup(gc, "|"), PIPE, gc));
		(*i) += 1;
	}
	else if (!ft_strncmp(&line[*i], ">>", 2)) {
		add_token(tokens, create_token_gc(gc_strdup(gc, ">>"), REDIR_APPEND, gc));
		
		(*i) += 2;
	}
	else if (!ft_strncmp(&line[*i], "<<", 2)) {
		add_token(tokens, create_token_gc(gc_strdup(gc, "<<"), HEREDOC, gc));
		(*i) += 2;
	}
	else if (line[*i] == '>') {
		add_token(tokens, create_token_gc(gc_strdup(gc, ">"), REDIR_OUT, gc));
		(*i) += 1;
	}
	else if (line[*i] == '<') {
		add_token(tokens, create_token_gc(gc_strdup(gc, "<"), REDIR_IN, gc));
		(*i) += 1;
	}
	else
		return (0);
	return (1);
}



/*
 * Process a word from the input line:
 * - Extracts the word (handles quotes)
 * - Expands variables if present
 * - Handles empty expansions (e.g., $UNSET_VAR)
 * - Splits the word if necessary (e.g., after expansion)
 * - Adds the resulting tokens as COMMAND or ARGUMENT
 * Returns 1 on success, 0 on error
 */
static int	process_word_gc(t_token **tokens, char *line, int *i, t_env *env, t_gc *gc, t_command *cmd)
{
	char	**split_words;
	t_token	*new;
	char	*expanded, *word;
	int		has_vars, j;
	t_token *last = NULL;
	if(*glbls)
	
	// exatract word between first quote "'""mmm""'" ====> first word is ' second mmm theard '
 	word = extract_word(line, i);
	if (!word)
		return (0);
	// if word with quote or no if no qoute return 1 ? 0
	has_vars = has_unquoted_variables(word);
	expanded = expand_variables(word, env, cmd);
	if (has_vars && (!expanded || !*expanded))
	{
		// If variable expansion results in empty, add a special empty token
		new = create_token_gc(gc_strdup(gc, ""), ARGUMENT, gc);
		if (!new)
			return (free(word), free(expanded), 0);
		new->is_empty_expansion = 1;
		add_token(tokens, new);
		free(word);
		free(expanded);
		return (1);
	}
	free(expanded);
	split_words = expand_and_split_gc(word, env, gc, cmd);
	free(word);
	if (!split_words)
		return (1);
	j = 0;
	while (split_words[j])
	{
		// The first word is considered a COMMAND, others are ARGUMENTs
		if (*tokens == NULL)
			new = create_token_gc(split_words[j], COMMAND, gc);
		else
			new = create_token_gc(split_words[j], ARGUMENT, gc);
		if (!new)
			return (0);
		add_token(tokens, new);
		j++;
	}
	return (1);
}



/*
 * Main tokenization function:
 * - Iterates through the input line
 * - Skips spaces
 * - Handles quoted words, special shell characters, and normal words
 * - Builds a linked list of tokens representing the parsed input
 * Returns the head of the token list, or NULL on error
 */
t_token	*tokenize_gc(char *line, t_env *env, t_gc *gc, t_command *cmd)
{
    int     i;
    t_token *tokens;
    
    i = 0;
    tokens = NULL;
    
    // Check for unclosed quotes before starting tokenization
    if (has_unclosed_quote(line))
    {
        print_error("unclosed quote", NULL);
        return (NULL);
    }
    
    while (line[i])
    {
		
        // Skip spaces
        if (!skip_spaces(line, &i))
            break;
        
        // Handle special characters (redirections, but NOT pipe)
        else if (handle_special_gc(&tokens, line, &i, gc))
            ; // index increment handled in handle_special_no_pipe_gc
        
        // Handle normal words and pipe
        else if (line[i] == '|')
        {
            add_token(&tokens, create_token_gc(gc_strdup(gc, "|"), PIPE, gc));
            i++;
        }
        else if (!process_word_gc(&tokens, line, &i, env, gc, cmd))
            return (NULL);
    }
    return (tokens);
}