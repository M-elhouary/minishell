#include "minishell.h"

int	handle_quotes(const char *line, int *i)
{
	char	quote;

	if (line[*i] != '\'' && line[*i] != '\"')
		return (0);
	quote = line[*i];
	(*i)++;
	while (line[*i] && line[*i] != quote)
	{
		if (line[*i] == '\\' && quote == '\"')
			(*i)++;
		if (line[*i])
			(*i)++;
	}
	if (!line[*i])
		return (print_error("unclosed quote", NULL), 0);
	(*i)++;
	return (1);
}