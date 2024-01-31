#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    printf("number of command-line arguments: %d\n", argc);

    // print each command line argument
    for (int i = 0; i < argc; i++) {
        printf("Argument %d: %s\n", i, argv[i]);
    }

    int pipefd[2];
    if (pipe(pipefd) == -1) {
        perror("pipe");
        return -1;
    }

    pid_t child_pid;

    // First child
    child_pid = fork();
    if (child_pid == -1) {
        perror("fork");
        return -1;
    }

    if (child_pid == 0) { // 1st child
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);
        execlp(argv[1], argv[1], NULL);
        perror("execlp");
        exit(EXIT_FAILURE);
    } else {
        wait(NULL); // Move wait outside the child block
        close(pipefd[1]);

        // Parent process
        dup2(pipefd[0], STDIN_FILENO);
        close(pipefd[0]);

        // Second child
        pipe(pipefd);
        child_pid = fork();
        if (child_pid == -1) {
            perror("fork");
            return -1;
        }

        if (child_pid == 0) { // 2nd child
            close(pipefd[0]);
            dup2(pipefd[1], STDOUT_FILENO);
            close(pipefd[1]);
            execlp(argv[2], argv[2], NULL);
            perror("execlp");
            exit(EXIT_FAILURE);
        } else {
            wait(NULL);
            close(pipefd[1]);

            // Parent process
            dup2(pipefd[0], STDIN_FILENO);
            close(pipefd[0]);

            // Third child
            execlp(argv[3], argv[3], NULL);
            perror("execlp");
            exit(EXIT_FAILURE);
        }
    }

    printf("This line should not be reached.\n");

    return 0;
}