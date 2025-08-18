#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    pid_t pid = fork();

    if (pid == 0) {
        // Child process: just exit immediately
        _exit(42);
    } else if (pid > 0) {
        int status;
        pid_t waited_pid = wait(&status);

        printf("Waited for PID: %d\n", waited_pid);

        if (WIFEXITED(status)) {
            printf("Child exited with status: %d\n", WEXITSTATUS(status));
        }
    } else {
        perror("fork");
        exit(1);
    }
    return 0;
}
