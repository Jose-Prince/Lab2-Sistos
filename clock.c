#include <stdio.h>
#include <time.h>

int main() {
    int i;
    clock_t first, last;

    first = clock();
    for (i = 0; i < 1000000; i++){
        printf("%d\n", i);
    }
    for (i = 0; i < 1000000; i++){
        printf("%d\n", i);
    }
    for (i = 0; i < 1000000; i++){
        printf("%d\n", i);
    }

    last = clock();

    double difference = (double)(last - first) / CLOCKS_PER_SEC;

    printf("%f", difference);

    return 0;
}
