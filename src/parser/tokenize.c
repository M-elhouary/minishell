#include "minishell.h"

static t_token	*create_token(char *value, t_token_type type)
{
	t_token	*new;

	new = malloc(sizeof(t_token));
	if (!new)
		return (NULL);
	new->value = value;
	new->type = type;
	new->is_empty_expansion = 0;
	new->next = NULL;
	return (new);
}

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

static int	handle_special(t_token **tokens, char *line, int *i)
{
	if (line[*i] == '|') {
		add_token(tokens, create_token(ft_strdup("|"), PIPE));
		(*i) += 1;
	}
	else if (!ft_strncmp(&line[*i], ">>", 2)) {
		add_token(tokens, create_token(ft_strdup(">>"), REDIR_APPEND));
		(*i) += 2;
	}
	else if (!ft_strncmp(&line[*i], "<<", 2)) {
		add_token(tokens, create_token(ft_strdup("<<"), HEREDOC));
		(*i) += 2;
	}
	else if (line[*i] == '>') {
		add_token(tokens, create_token(ft_strdup(">"), REDIR_OUT));
		(*i) += 1;
	}
	else if (line[*i] == '<') {
		add_token(tokens, create_token(ft_strdup("<"), REDIR_IN));
		(*i) += 1;
	}
	else
		return (0);
	return (1);
}

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

static int	process_word(t_token **tokens, char *line, int *i, t_env *env)
{
	char	*word;
	char	**split_words;
	t_token	*new;
	int		j;
	char	*expanded;
	int		has_vars;

	word = extract_word(line, i);
	if (!word)
		return (0);
	has_vars = has_unquoted_variables(word);
	expanded = expand_variables(word, env);
	if (has_vars && (!expanded || !*expanded))
	{
		new = create_token(ft_strdup(""), ARGUMENT);
		if (!new)
			return (free(word), free(expanded), 0);
		new->is_empty_expansion = 1;
		add_token(tokens, new);
		free(word);
		free(expanded);
		return (1);
	}
	free(expanded);
	split_words = expand_and_split(word, env);
	free(word);
	if (!split_words)
		return (1);
	j = 0;
	while (split_words[j])
	{
		if (*tokens == NULL)
			new = create_token(split_words[j], COMMAND);
		else
			new = create_token(split_words[j], ARGUMENT);
		if (!new)
		{
			while (split_words[j])
				free(split_words[j++]);
			free(split_words);
			return (0);
		}
		add_token(tokens, new);
		j++;
	}
	free(split_words);
	return (1);
}

static int	process_word_gc(t_token **tokens, char *line, int *i, t_env *env, t_gc *gc)
{
	char	*word;
	char	**split_words;
	t_token	*new;
	int		j;
	char	*expanded;
	int		has_vars;

	word = extract_word(line, i);
	if (!word)
		return (0);
	has_vars = has_unquoted_variables(word);
	expanded = expand_variables(word, env);
	if (has_vars && (!expanded || !*expanded))
	{
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
	split_words = expand_and_split_gc(word, env, gc);
	free(word);
	if (!split_words)
		return (1);
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

t_token	*tokenize(char *line, t_env *env)
{
	int		i;
	t_token	*tokens;

	i = 0;
	tokens = NULL;
	while (line[i])
	{
		if (!skip_spaces(line, &i))
			break ;
		if (line[i] == '\'' || line[i] == '"')
		{
			if (!process_word(&tokens, line, &i, env))
				return (free_token(tokens), NULL);
		}
		else if (handle_special(&tokens, line, &i))
			; // index increment handled in handle_special
		else if (!process_word(&tokens, line, &i, env))
			return (free_token(tokens), NULL);
	}
	return (tokens);
}

t_token	*tokenize_gc(char *line, t_env *env, t_gc *gc)
{
	int		i;
	t_token	*tokens;

	i = 0;
	tokens = NULL;
	while (line[i])
	{
		if (!skip_spaces(line, &i))
			break ;
		if (line[i] == '\'' || line[i] == '"')
		{
			if (!process_word_gc(&tokens, line, &i, env, gc))
				return (NULL);
		}
		else if (handle_special_gc(&tokens, line, &i, gc))
			; // index increment handled in handle_special_gc
		else if (!process_word_gc(&tokens, line, &i, env, gc))
			return (NULL);
	}
	return (tokens);
}