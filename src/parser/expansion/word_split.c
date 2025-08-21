#include "minishell.h"

static int	count_words_in_expanded(const char *str)
{
	int	count;
	int	in_word;
	int	i;

	count = 0;
	in_word = 0;
	i = 0;
	while (str[i])
	{
		if (!is_space(str[i]) && !in_word)
		{
			in_word = 1;
			count++;
		}
		else if (is_space(str[i]))
			in_word = 0;
		i++;
	}
	return (count);
}



static char	**split_expanded_word_gc(const char *expanded, t_gc *gc)
{
	char	**words;
	int		word_count;
	int		start;

	int i, j;
	word_count = count_words_in_expanded(expanded);
	if (word_count == 0)
		return (NULL);
	words = gc_malloc(gc, sizeof(char *) * (word_count + 1));
	if (!words)
		return (NULL);
	i = 0;
	j = 0;
	while (expanded[i] && j < word_count)
	{
		while (expanded[i] && is_space(expanded[i]))
			i++;
		start = i;
		while (expanded[i] && !is_space(expanded[i]))
			i++;
		if (i > start)
			words[j++] = gc_strndup(gc, expanded + start, i - start);
	}
	words[j] = NULL;
	return (words);
}


char	**expand_and_split_gc_updated(const char *word, t_env *env, t_gc *gc, t_command *cmd)
{
	char	*expanded;
	char	**split_words;
	char	**result;

	if (!has_unquoted_variables(word))
	{
		expanded = expand_variables_gc(word, env, cmd, gc);
		result = gc_malloc(gc, sizeof(char *) * 2); 
		if (!result)
			return (NULL);
		result[0] = expanded;
		result[1] = NULL;
		return (result);
	}
	expanded = expand_variables_gc(word, env, cmd, gc);
	if (!expanded || !*expanded)
	{
		return (NULL);
	}
	split_words = split_expanded_word_gc(expanded, gc);
	return (split_words);
}
