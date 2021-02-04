#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUMITERS 10000000
int shared_var = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *adder(void *param) {
    for (int i = 0; i < NUMITERS; i++) {
        pthread_mutex_lock(&mutex);
        shared_var++;
        pthread_mutex_unlock(&mutex);
    }
    pthread_exit(0);
}

void *suber(void *param) {
    for (int i = 0; i < NUMITERS; i++) {
        pthread_mutex_lock(&mutex);
        shared_var--;
        pthread_mutex_unlock(&mutex);

    }
    pthread_exit(0);
}

int main(void) {

    pthread_t tids[2];

    pthread_create(&tids[0], NULL, adder, NULL);
    pthread_create(&tids[1], NULL, suber, NULL);

    for (int i = 0; i < 2; i++) {
        pthread_join(tids[i], NULL);
    }

    printf("shared_var = %d\n", shared_var);
}

