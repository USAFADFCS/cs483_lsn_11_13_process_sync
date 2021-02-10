#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define NUMITER 5

int shared_var = 0;
sem_t x, y, z;


void *a(void *param) {

    for (int i = 0; i < NUMITER; i++) {
        sem_wait(&x);
        printf("Thread A - In CS\n");
        fflush(stdout);
        shared_var++;
        sem_post(&y);
    }
    pthread_exit(0);
}

void *b(void *param) {

    for (int i = 0; i < NUMITER; i++) {
        sem_wait(&y);
        printf("Thread B - In CS\n");
        fflush(stdout);
        shared_var++;
        sem_post(&z);
    }
    pthread_exit(0);
}

void *c(void *param) {

    for (int i = 0; i < NUMITER; i++) {
        sem_wait(&z);
        printf("Thread C - In CS\n");
        fflush(stdout);
        shared_var++;
        sem_post(&x);
    }
    pthread_exit(0);
}

int main(void) {

    pthread_t tids[3];

    sem_init(&x, 0, 1);
    sem_init(&y, 0, 0);
    sem_init(&z, 0, 0);

    pthread_create(&tids[0], NULL, a, NULL);
    pthread_create(&tids[1], NULL, b, NULL);
    pthread_create(&tids[2], NULL, c, NULL);

    for (int i = 0; i < 3; i++) {
        pthread_join(tids[i], NULL);
    }

    printf("shared_var = %d\n", shared_var);
}