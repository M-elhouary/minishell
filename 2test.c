#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int	main()
{
	int pfd[2];
	pipe(pfd);
	printf("%d %d\n", pfd[0], pfd[1]);
	pid_t pid = fork();
	if (pid == 0)
	{
		dup2(pfd[1], STDOUT_FILENO);
	}

	return 0;
}