#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int main()
{
    printf("parent: [%d], ppid: %d\n", getpid(), getppid());

    int pid = fork();
    if (pid == 0)
    {
        // Child process
        printf("child: %d, parent: %d\n", getpid(), getppid());
        sleep(3);  // Child continues living
        printf("child: %d, NEW parent: %d\n", getpid(), getppid());
        printf("child finishing: %d\n", getpid());
		// exit(1);
    }
    else if (pid > 0)
    {
        // Parent process exits immediately
		sleep(2);
        printf("parent exiting: %d\n", getpid());
        // No wait() - parent dies first!
    }
    // wait(NULL);
    return 0;
}