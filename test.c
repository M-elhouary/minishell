#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main() {
    int pipe1[2], pipe2[2];
    pipe(pipe1);  // between cat and grep
    pipe(pipe2);  // between grep and sort

    pid_t pid1 = fork();
    if (pid1 == 0) {
        // cat process
        dup2(pipe1[1], STDOUT_FILENO);
        close(pipe1[0]); close(pipe1[1]);
        close(pipe2[0]); close(pipe2[1]);  // not used
        char *cmd1[] = {"/bin/cat", "file.txt", NULL};
        execv(cmd1[0], cmd1);
        perror("execv cat"); exit(1);
    }

    pid_t pid2 = fork();
    if (pid2 == 0) {
        // grep process
        dup2(pipe1[0], STDIN_FILENO);
        dup2(pipe2[1], STDOUT_FILENO);
        close(pipe1[0]); close(pipe1[1]);
        close(pipe2[0]); close(pipe2[1]);
        char *cmd2[] = {"/bin/grep", "keyword", NULL};
        execv(cmd2[0], cmd2);
        perror("execv grep"); exit(1);
    }

    pid_t pid3 = fork();
    if (pid3 == 0) {
        // sort process
        dup2(pipe2[0], STDIN_FILENO);
        close(pipe2[0]); close(pipe2[1]);
        close(pipe1[0]); close(pipe1[1]);  // not used
        char *cmd3[] = {"/usr/bin/sort", NULL};
        execv(cmd3[0], cmd3);
        perror("execv sort"); exit(1);
    }

    // Parent closes all pipe ends and waits
    close(pipe1[0]); close(pipe1[1]);
    close(pipe2[0]); close(pipe2[1]);
    wait(NULL); wait(NULL); wait(NULL);

    return 0;
}















