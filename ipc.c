#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>

#define SHM_NAME "/my_shared_memory"
#define SHM_SIZE 50

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Uso: %s <número> <letra>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int n = atoi(argv[1]);
    char x = argv[2][0];

    printf("I am %c\n", x);

    int fd, is_creator = 0;
    char *shm_ptr;

    fd = shm_open(SHM_NAME, O_RDWR, 0666);
    if (fd == -1) {
        if (errno == ENOENT) {
            fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
            if (fd == -1) {
                perror("Error al crear la memoria compartida");
                exit(EXIT_FAILURE);
            }
            is_creator = 1;
            ftruncate(fd, SHM_SIZE);
            printf("%c: Create new shared mem obj %d\n", x, fd);
        } else {
            perror("Error al abrir memoria compartida");
            exit(EXIT_FAILURE);
        }
    } else {
        printf("%c: Shared mem obj already created\n", x);
    }

    shm_ptr = mmap(NULL, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (shm_ptr == MAP_FAILED) {
        perror("Error en mmap");
        exit(EXIT_FAILURE);
    }
    printf("%c: Ptr created with value %p\n", x, shm_ptr);

    if (is_creator) {
        memset(shm_ptr, '-', SHM_SIZE);
        printf("%c: Initialized shared mem obj\n", x);
    }

    int pipefd[2];
    if (pipe(pipefd) == -1) {
        perror("Error al crear pipe");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();
    if (pid == -1) {
        perror("Error en fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        close(pipefd[1]);
        int index;
        while (read(pipefd[0], &index, sizeof(int)) > 0) {
            if (index < SHM_SIZE) {
                shm_ptr[index] = x;
            }
        }
        close(pipefd[0]);
        exit(EXIT_SUCCESS);
    } else {
        close(pipefd[0]);
        for (int i = 0; i < SHM_SIZE; i++) {
            if (i % n == 0) {
                write(pipefd[1], &i, sizeof(int));
            }
        }
        close(pipefd[1]);
        wait(NULL);

        printf("%c: Shared memory has: ", x);
        write(STDOUT_FILENO, shm_ptr, SHM_SIZE);
        printf("\n");

        munmap(shm_ptr, SHM_SIZE);
        if (is_creator) {
            shm_unlink(SHM_NAME);
            printf("%c: Shared memory removed.\n", x);
        }
    }

    return 0;
}

