#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t id;
    clock_t first, last;

    int i;

    first = clock();
    id = fork();

    if (id == 0) {
        id = fork();
        
        if (id == 0) {
            id = fork();
                
            if (id == 0) {
                for (i = 0; i < 1000000; i++) {
                    printf("%d\n", i);
                }
            } else {
                for (i = 0; i < 1000000; i++) {
                    printf("%d\n", i);
                }
                wait(NULL);
            } 
        } else {
            for (i = 0; i < 1000000; i++) {
                printf("%d\n", i);
            }
            wait(NULL);
        }
    } else {
        wait(NULL);
        last = clock();

        double total = (double)(last - first) / CLOCKS_PER_SEC;
        printf("%f", total);
    }

    return 0;
}
