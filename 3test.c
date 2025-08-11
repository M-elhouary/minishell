#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>

// int main()
// {
// 	pid_t pid;
// 	pid = fork();
// 	if (pid == 0)
// 	{
// 		int fd = open("3.txt",O_RDWR);
// 		char buff[10];
// 		write(fd, "child data", 10);
// 		close(fd);
// 		fd = open("3.txt",O_RDWR);
// 		read(fd, buff, 10);
// 		close(fd);
// 		printf("%s\n", buff);
// 	}
// 	else
// 	{
// 		int fd = open("3.txt",O_RDWR);
// 		char buff[11];
// 		write(fd, "parent data", 11);
// 		close(fd);
// 		fd = open("3.txt",O_RDWR);
// 		read(fd, buff, 11);
// 		close(fd);
// 		printf("%s\n", buff);
// 	}
// 	wait(&pid);
// }

// int main() {
//     int fd = open("test.txt", O_RDWR | O_CREAT | O_TRUNC, 0644);
//     //  ^^ Single fd shared between processes
    
//     if (fork() == 0) {
//         write(fd, "child", 5);
//         lseek(fd, 0, SEEK_SET);  // Reset to read
//         char buf[20];
//         read(fd, buf, 5);
//         printf("Child read: %s\n", buf);
//     } else {
//         sleep(1);  // Let child write first
//         write(fd, "parent", 6);  // This might overwrite or append
//         lseek(fd, 0, SEEK_SET);
//         char buf[20];
//         read(fd, buf, 6);
//         printf("Parent read: %s\n", buf);
//         wait(NULL);
//     }
//     close(fd);
// }

// int main() {
//     // Simulate two-fd pipe using two separate files
//     int write_fd = open("pipe_write.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
//     int read_fd = open("pipe_write.txt", O_RDONLY);
    
//     if (fork() == 0) {
//         // Child process - WRITER ONLY
//         close(read_fd);  // Child doesn't read
        
//         write(write_fd, "child data", 10);
//         printf("Child wrote: child data\n");
//         close(write_fd);  // Signal "no more data"
        
//     } else {
//         // Parent process - READER ONLY  
//         close(write_fd);  // Parent doesn't write
        
//         sleep(1);  // Wait for child to write
        
//         char buf[20];
//         int bytes_read;
//         while ((bytes_read = read(read_fd, buf, sizeof(buf)-1)) > 0) {
//             buf[bytes_read] = '\0';
//             printf("Parent read: %s\n", buf);
//         }
        
//         if (bytes_read == 0) {
//             printf("Parent got EOF (no more data)\n");
//         }
        
//         close(read_fd);
//         wait(NULL);
//     }
    
//     return 0;
// }

// int main() {
//     int fd[2];
//     pipe(fd);
    
//     if (fork() == 0) {
//         // Child - WRITER
//         close(fd[0]);  // Don't need read-end
//         write(fd[1], "child data", 10);
//         printf("Child wrote: child data\n");
//         close(fd[1]);  // Signals EOF to parent
        
//     } else {
//         // Parent - READER  
//         close(fd[1]);  // Don't need write-end
        
//         char buf[20];
//         int bytes_read = read(fd[0], buf, 10);
        
//         if (bytes_read > 0) {
//             buf[bytes_read] = '\0';
//             printf("Parent read: %s\n", buf);
//         }
        
//         // Try to read more
//         bytes_read = read(fd[0], buf, 10);
//         if (bytes_read == 0) {
//             printf("Parent got EOF (child closed write-end)\n");
//         }
        
//         close(fd[0]);
//         wait(NULL);
//     }
// }

// int main() {
//     int fd = open("test.txt", O_RDWR | O_CREAT | O_TRUNC, 0644);
//     // ^^ Single shared fd between processes
    
//     if (fork() == 0) {
//         // Child process - WRITER ONLY
//         write(fd, "child data", 10);
//         printf("Child wrote: child data\n");
//         // fd offset is now at position 10
        
//     } else {
//         // Parent process - READER ONLY
//         sleep(1);  // Wait for child to write
        
//         char buf[20];
//         // fd offset is at position 10 (shared with child!)
//         // int bytes_read = read(fd, buf, 10);
        
//         // if (bytes_read == 0) {
//         //     printf("Parent read 0 bytes (EOF - nothing to read at offset 10)\n");
//         // } else {
//         //     buf[bytes_read] = '\0';
//         //     printf("Parent read: %s (%d bytes)\n", buf, bytes_read);
//         // }
        
//         // To actually read the data, parent must reset offset:
//         lseek(fd, 0, SEEK_SET);  // Reset to beginning
//         int bytes_read = read(fd, buf, 10);
//         buf[bytes_read] = '\0';
//         printf("After lseek(0): Parent read: %s\n", buf);
        
//         close(fd);
//         wait(NULL);
//     }
    
//     return 0;
// }

