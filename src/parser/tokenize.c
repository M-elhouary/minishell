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
	if (line[*i] == '|')
	{

		add_token(tokens, create_token(ft_strdup("|"), PIPE));
		return 1;
	}
	else if (!ft_strncmp(&line[*i], ">>", 2))
	{
		add_token(tokens, create_token(ft_strdup(">>"), REDIR_APPEND));
		return 2;

	}
	else if (!ft_strncmp(&line[*i], "<<", 2))
	{
		add_token(tokens, create_token(ft_strdup("<<"), HEREDOC));
		return 2;

	}
	else if (line[*i] == '>')
	{
		add_token(tokens, create_token(ft_strdup(">"), REDIR_OUT));
		return 1;

	}
	else if (line[*i] == '<')
	{
		add_token(tokens, create_token(ft_strdup("<"), REDIR_IN));
		return 1;

	}
	else
		return (0);
}

static int	process_word(t_token **tokens, char *line, int *i, t_env *env, t_flags *flags)
{
	char	*word;
	t_token	*new;
	t_token_type	type;

	if (*tokens == NULL) //
		type = WORD;
	else
		type = ARGUMENT;
	word = extract_word(line, i, flags);
	if (!word)
		return (0);
	if (!(word[0] == '\0' && (line[*i-1] == '\'' || line[*i-1] == '"')))
		word = remove_quotes(word);
	if (ft_strchr(word, '$') && flags->f_squote == 0)
	{
		word = expand_variables(word, env);
		if (!word)
			return (0);
	}
	new = create_token(word, type);
	if (!new)
		return (free(word), 0);
	add_token(tokens, new);
	return (1);
}

t_token	*tokenize(char *line, t_env *env, t_flags *flags)
{
	int		i;
	int incremt;
	t_token	*tokens;

	i = 0;
	tokens = NULL;
	while (line[i])
	{
		if (!skip_spaces(line, &i))
			break ;
		if (line[i] == '\'' || line[i] == '"')
		{
			if (!process_word(&tokens, line, &i, env, flags))
				return (free_token(tokens), NULL);
		}
		else if ((incremt = handle_special(&tokens, line, &i)))
		{
			i += incremt;
		}
		else if (!process_word(&tokens, line, &i, env, flags))
			return (free_token(tokens), NULL);
	}
	return (tokens);
}