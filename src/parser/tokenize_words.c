#include "minishell.h"

static int	handle_word_quotes(const char *line, int *i, char quote)
{
	(*i)++;
	while (line[*i] && line[*i] != quote)
	{
		if (line[*i] == '\\' && quote == '\"')
			(*i)++;
		if (line[*i])
			(*i)++;
	}
	if (!line[*i])
		return (print_error("unclosed quote in word", NULL), 0);
	(*i)++;
	return (1);
}

char	*remove_quotes(char *str)
{
	char	*new;
	int		i;
	int		j;

	i = 0;
	j = 0;
	if (!str)
		return (NULL);
	new = malloc(ft_strlen(str) + 1);
	if (!new)
		return (NULL);
	while (str[i])
	{
		if (str[i] != '\'' && str[i] != '\"')
			new[j++] = str[i];
		i++;
	}
	new[j] = '\0';
	free(str);
	return (new);
}

char	*extract_word(const char *line, int *i)
{
	int		start;
	char	*word;
	char	quote;

	start = *i;
	while (line[*i] && !is_metacharacter(line[*i]))
	{
		if (line[*i] == '\'' || line[*i] == '\"')
		{
			quote = line[*i];
			if (!handle_word_quotes(line, i, quote))
				return (NULL);
		}
		else
			(*i)++;
	}
	if (*i - start == 0 && (line[start-1] == '\'' || line[start-1] == '"'))
		return (ft_strdup(""));
	word = ft_strndup(line + start, *i - start);
	if (!word)
		print_error("memory allocation failed", NULL);
	return (word);
}