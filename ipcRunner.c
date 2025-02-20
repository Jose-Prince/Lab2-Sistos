#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t pid1, pid2;

    pid1 = fork();
    if (pid1 == -1) {
        perror("Error en fork 1");
        exit(EXIT_FAILURE);
    }

    if (pid1 == 0) {
        execl("./ipc", "ipc", "3", "A", NULL);
        perror("Error en execl (A)");
        exit(EXIT_FAILURE);
    }

    pid2 = fork();
    if (pid2 == -1) {
        perror("Error en fork 2");
        exit(EXIT_FAILURE);
    }

    if (pid2 == 0) {
        execl("./ipc", "ipc", "5", "B", NULL);
        perror("Error en execl (B)");
        exit(EXIT_FAILURE);
    }

    wait(NULL);
    wait(NULL);

    return 0;
}
