// Handle single command without pipeline
static int	execute_single_command(t_command *cmd_list, t_env **env)
{
	cmd_list->print_exit = 1;
	return (exec_cmd(cmd_list, env, STDOUT_FILENO));
}

// Set signals to ignore, return old handlers
static void	set_parent_signals(void (**old_int)(int), void (**old_quit)(int))
{
	*old_int = signal(SIGINT, SIG_IGN);
	*old_quit = signal(SIGQUIT, SIG_IGN);
}

// Restore original parent signals
static void	restore_parent_signals(void (*old_int)(int), void (*old_quit)(int))
{
	signal(SIGINT, old_int);
	signal(SIGQUIT, old_quit);
}

// Allocate space for PIDs, restore signals on failure
static pid_t	*allocate_pids(int cmd_count, void (*old_int)(int), void (*old_quit)(int))
{
	pid_t *pids = malloc(sizeof(pid_t) * cmd_count);
	if (!pids)
		restore_parent_signals(old_int, old_quit);
	return (pids);
}

// Handle full pipeline execution
static int	execute_pipeline_commands(t_command *cmd_list, t_env **env,
										pid_t *pids, int cmd_count,
										void (*old_int)(int), void (*old_quit)(int))
{
	int	processes_created;
	int	exit_status_code;

	processes_created = execute_pipeline_loop(cmd_list, env, pids, cmd_count);
	if (processes_created == -1)
	{
		cleanup_pipeline_resources(pids, NULL, cmd_count);
		restore_parent_signals(old_int, old_quit);
		return (1);
	}
	exit_status_code = wait_for_children(pids, processes_created);
	cleanup_pipeline_resources(pids, NULL, cmd_count);
	restore_parent_signals(old_int, old_quit);
	return (exit_status_code);
}

// Main entry point
int	exec_pipeline(t_command *cmd_list, t_env **env)
{
	int		cmd_count;
	pid_t	*pids;
	void	(*old_int)(int);
	void	(*old_quit)(int);

	cmd_count = count_cmds(cmd_list);
	if (cmd_count == 1)
		return (execute_single_command(cmd_list, env));

	set_parent_signals(&old_int, &old_quit);
	pids = allocate_pids(cmd_count, old_int, old_quit);
	if (!pids)
		return (1);

	return (execute_pipeline_commands(cmd_list, env, pids, cmd_count, old_int, old_quit));
}
