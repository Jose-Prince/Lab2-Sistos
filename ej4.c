#include <stdio.h>
#include <unistd.h>

int main() {
    pid_t id = fork();
    int i;

    if (id == 0) {
        for (i = 1; i  < 4000000; i++) {
            printf("%d", i);
        }
    } else {
        while (1) {
        }
    }

    return 0;
}
