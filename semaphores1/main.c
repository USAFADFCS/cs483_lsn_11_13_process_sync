/* The purpose of this example is to show how semaphores can be utilized as a signaling mechanism
 * to enforces thread execution ordering. In this example, we can guarantee that A will run before
 * B which will run before C
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

int shared_var = 0;
sem_t x,y;


void *a(void *param) {
    for (int i = 0; i < 3; i++) {
        printf("Thread A - In CS\n");
        fflush(stdout);
        shared_var++;

    }
    sem_post(&x);
    pthread_exit(0);
}

void *b(void *param) {

    sem_wait(&x);
    for (int i = 0; i < 2; i++) {
        printf("Thread B - In CS\n");
        fflush(stdout);
        shared_var++;
    }
    sem_post(&y);
    pthread_exit(0);
}

void *c(void *param) {

    sem_wait(&y);
    printf("Thread C - In CS\n");
    fflush(stdout);
    shared_var++;
    pthread_exit(0);
}

int main(void) {

    pthread_t tids[3];

    sem_init(&x,0,0);
    sem_init(&y,0,0);

    pthread_create(&tids[0], NULL, a, NULL);
    pthread_create(&tids[1], NULL, b, NULL);
    pthread_create(&tids[2], NULL, c, NULL);

    for (int i = 0; i < 3; i++) {
        pthread_join(tids[i], NULL);
    }

    printf("shared_var = %d\n", shared_var);
}
