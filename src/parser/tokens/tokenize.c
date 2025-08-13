/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houa <mel-houa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 20:03:03 by mel-houa          #+#    #+#             */
/*   Updated: 2025/08/13 01:00:12 by mel-houa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"





// Helper function to handle double-character operators
static int handle_double_char_op(t_token **tokens, char *line, int *i, t_gc *gc)
{
    if (!ft_strncmp(&line[*i], ">>", 2))
    {
        create_and_add_token(tokens, ">>", REDIR_APPEND, gc);
        (*i) += 2;
        return (1);
    }
    else if (!ft_strncmp(&line[*i], "<<", 2))
    {
        create_and_add_token(tokens, "<<", HEREDOC, gc);
        (*i) += 2;
        return (1);
    }
    return (0);
}

// Handle special shell characters
static int handle_special_gc(t_token **tokens, char *line, int *i, t_gc *gc)
{
    // First check for double-character operators
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
static int add_split_words(t_token **tokens, char **split_words, t_gc *gc)
{
    t_token *new;
    int j;

    j = 0;
    while (split_words[j])
    {
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

// Process word and create tokens
static int process_word_gc(char *line, int *i, t_token_glbst *glbst)
{
    char **split_words;
    char *expanded, *word;
    int has_vars;
    t_token *last = NULL;

    if (*glbst->tokens)
    {
        last = *glbst->tokens;
        while (last->next)
            last = last->next;
    }
    word = extract_word(line, i);
    if (!word)
        return (0);
    
    if(last && last->type == HEREDOC)
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
    has_vars = has_unquoted_variables(word);
    expanded = expand_variables(word, glbst->env, glbst->cmd);
    if (has_vars && (!expanded || !*expanded))
    {
        free(word);
        free(expanded);
        return (handle_empty_expansion(glbst->tokens, glbst->gc));
    }
    free(expanded);
    split_words = expand_and_split_gc(word, glbst->env, glbst->gc, glbst->cmd);
    free(word);
    if (!split_words)
        return (1);
    return (add_split_words(glbst->tokens, split_words, glbst->gc));
}

// Process special characters in tokenization
static int process_special(char *line, int *i, t_token_glbst *glbst)
{
    // Skip spaces
    if (!skip_spaces(line, i))
        return (0);
    
    // Handle special characters
    if (handle_special_gc(glbst->tokens, line, i, glbst->gc))
        return (1);
    
    // Handle pipe
    if (line[*i] == '|')
    {
        add_token(glbst->tokens, create_token_gc(
            gc_strdup(glbst->gc, "|"), PIPE, glbst->gc));
        (*i)++;
        return (1);
    }
    
    // Handle regular word
    return (process_word_gc(line, i, glbst));
}



// Main tokenization function
t_token *tokenize_gc(char *line, t_env *env, t_gc *gc, t_command *cmd)
{
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
            break;
    }
    
    return (*(glbst.tokens));
}