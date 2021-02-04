#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUMITERS 1000000000
int shared_var = 0;

void *adder(void *param) {
    for (int i = 0; i < NUMITERS; i++) {
        shared_var++;
    }
    pthread_exit(0);
}

void *subber(void *param) {
    for (int i = 0; i < NUMITERS; i++) {
        shared_var--;
    }
    pthread_exit(0);
}

int main(void) {

    pthread_t tids[2];

    pthread_create(&tids[0], NULL, adder, NULL);
    pthread_create(&tids[1], NULL, subber, NULL);

    for (int i = 0; i < 2; i++) {
        pthread_join(tids[i], NULL);
    }

    printf("shared_var = %d\n", shared_var);
}

