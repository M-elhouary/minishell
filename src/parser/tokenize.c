#include "minishell.h"

static t_token	*create_token(char *value, t_token_type type)
{
	t_token	*new;

	new = malloc(sizeof(t_token));
	if (!new)
		return (NULL);
	new->value = value;
	new->type = type;
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

static int	process_word(t_token **tokens, char *line, int *i, t_env *env)
{
	char	*word;
	t_token	*new;

	word = extract_word(line, i);
	if (!word)
		return (0);
	
	// Always expand variables and handle quotes in expand_variables
	word = expand_variables(word, env);
	if (!word)
		return (0);
	if (*tokens == NULL)
		new = create_token(word, COMMAND); // first token is COMMAND
	else
		new = create_token(word, ARGUMENT); // all others are ARGUMENT
	new = create_token(word, (*tokens) ? ARGUMENT : COMMAND);
	if (!new)
		return (free(word), 0);
	add_token(tokens, new);
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