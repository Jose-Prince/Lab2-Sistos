#include <stdio.h>
#include <unistd.h>

int main() {
    fork();
    fork();
    fork();
    fork();

    printf("Hello World\n");

    return 0;
}
