#include "minishell.h"

<<<<<<< HEAD
static int	handle_word_quotes(const char *line, int *i, char quote)
{
	(*i)++;
	while (line[*i] && line[*i] != quote)
	{
		// if (line[*i] == '\\' && quote == '\"')
		// 	(*i)++;
			(*i)++;
	}
	if (!line[*i])
		return (print_error("unclosed quote in word", NULL), 0);
	(*i)++;
	return (1);
}
=======
// static int	handle_word_quotes(const char *line, int *i, char quote)
// {
// 	(*i)++;
// 	while (line[*i] && line[*i] != quote)
// 	{
// 		if (line[*i] == '\\' && quote == '\"')
// 			(*i)++;
// 		if (line[*i])
// 			(*i)++;
// 	}
// 	if (!line[*i])
// 		return (print_error("unclosed quote in word", NULL), 0);
// 	(*i)++;
// 	return (1);
// }
>>>>>>> 002946b (update expand with quote)

char	*remove_quotes(char *str)
{
	char	*new;
	int		i;
	int		j;

	i = 1;
	j = 0;
	if (!str)
		return (NULL);
	new = malloc(ft_strlen(str) + 1);
	if (!new)
		return (NULL);
	while (str[i])
	{
		//if (str[i] != '\'' && str[i] != '\"')
			new[j++] = str[i];
		i++;
	}
	new[j] = '\0';
	free(str);
	return (new);
}static int	skip_quoted_section(const char *line, int *i, char quote)
{
	(*i)++; // Skip opening quote
	while (line[*i] && line[*i] != quote)
		(*i)++;
	if (!line[*i])
		return (0); // Unclosed quote
	(*i)++; // Skip closing quote
	return (1);
}

char	*extract_word(const char *line, int *i, t_flags *flags)
{
	int		start;
	char	*word;


	start = *i;
	while (line[*i] && !is_metacharacter(line[*i]))
	{
		if (line[*i] == '\'' || line[*i] == '"')
		{
<<<<<<< HEAD
			if(line[*i] == '\'' && line[*i+1] != '\"' )
					flags->f_squote = 1;
			quote = line[*i];
			if (!handle_word_quotes(line, i, quote))
				return (NULL);
=======
			if (!skip_quoted_section(line, i, line[*i]))
				break; // Unclosed quote, stop here
>>>>>>> 002946b (update expand with quote)
		}
		else
			(*i)++;
	}
<<<<<<< HEAD
	if (*i - start == 0 && (line[start-1] == '\'' || line[start-1] == '"'))
		return (ft_strdup(""));
	word = ft_strndup(line + start, *i - start - 1);
	if (!word)
		print_error("memory allocation failed", NULL);
=======
	word = ft_strndup(line + start, *i - start);
>>>>>>> 002946b (update expand with quote)
	return (word);
}