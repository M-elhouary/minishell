#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <strings.h>
#include <fcntl.h>
#include <sys/wait.h>

int main()
{
	char buff[128];
	while (1)
	{
		int pfd[2];
		pid_t pid;
		if (pipe(pfd) == -1)
		{
			perror("pipe");
			exit(EXIT_FAILURE);
		}
		pid = fork();
		if (pid == -1)
		{
			perror("fork");
			return (EXIT_FAILURE);
		}
		if (pid == 0)
		{
			close(pfd[0]);
			//fflush(stdout);
			// write(stdout)
			dup2(pfd[1], STDOUT_FILENO);
			close(pfd[1]);
			write(STDIN_FILENO, "write in child: ", 16);
			//printf("write in child: ");
			ssize_t n = read(STDIN_FILENO, buff, sizeof(buff) - 1);
			if (n)
			{
				buff[n] = '\0';
				write(STDOUT_FILENO, buff, n);
			}
			exit(EXIT_SUCCESS);
		}
		else
		{
			close(pfd[1]);
			ssize_t n = read(pfd[0], buff, sizeof(buff) - 1);
			if (n)
			{
				buff[n] = '\0';
				printf("parent received: %s", buff);
			}
			close(pfd[0]);
			wait(NULL);
		}
	}
	return (0);
}
