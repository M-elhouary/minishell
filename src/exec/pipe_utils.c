#include "minishell.h"

int	count_cmds(t_command *cmd)
{
	int	count;

	count = 0;
	while (cmd)
	{
		count++;
		cmd = cmd->next;
	}
	return (count);
}

int	open_next_pipe(int is_last, int curr_pipe[2])
{
	if (is_last)
	{
		curr_pipe[0] = -1;
		curr_pipe[1] = -1;
		return (0);
	}
	if (pipe(curr_pipe) == -1)
	{
		perror("pipe");
		return (-1);
	}
	return (0);
}
