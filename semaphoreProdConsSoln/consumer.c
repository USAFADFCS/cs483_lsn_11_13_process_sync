#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <time.h>

/*
 * Note: In order for CMake to properly link this program, we need to add the following
 * to the end of CMakeLists.txt:
 *
 * target_link_libraries([your target name...whatever you have inside add_executable()] "-lrt")
 *
 * This will include the required libraries for shared memory functionality
 */

int main() {
    const int SIZE = 4096;
    const char *name = "OS";
    const char *semNameNumWrite = "semaphoreNumWrite";
    const char *semNameNumRead = "semaphoreNumRead";
    const char *semNameMsg = "semaphoreMsg";
    int shm_fd;
    void *ptr;
    void *buf_start; //Track the beginning of the shared buffer
    sem_t *sem_ptr_num_write; //Ready to write to guessed number buffer
    sem_t *sem_ptr_msg;
    sem_t *sem_ptr_num_read; //Ready to read from guessed number buffer
    int notWinning = 1;
    int userGuess;
    char userGuessString[50];
    srand(time(NULL));
    int actualNumber = rand() % 15;

    //Take this out if you want the game to be challenging
    printf("The magic number is: %d\n",actualNumber);
    /* open the shared memory segment */
    shm_fd = shm_open(name, O_RDWR, 0666);
    sem_ptr_num_write = sem_open(semNameNumWrite, O_CREAT, 0666);
    sem_ptr_num_read = sem_open(semNameNumRead, O_CREAT, 0666);
    sem_ptr_msg = sem_open(semNameMsg, O_CREAT, 0666);

    if (shm_fd == -1) {
        printf("Shared memory failed.\n");
        exit(-1);
    }

    /* now map the shared memory segment in the address space of the process */
    ptr = mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    buf_start = ptr;

    if (ptr == MAP_FAILED) {
        printf("Map failed.\n");
        return -1;
    }

    /* Now read from the shared memory region */
    //printf("%s\n", (char *)ptr);
    // Game code

    while(notWinning) {
        //Wait for a signal that the shared buffer is ready to be read from.
        sem_wait((sem_t *) sem_ptr_num_read);
        sprintf(userGuessString,"%s",(char *) ptr); //Read the number from the buffer Write = 0, Read = 0
        sem_post((sem_t *) sem_ptr_num_write);
        userGuess = atoi(userGuessString); //Read from the shared buffer
        ptr += sizeof((char) userGuess + 1);//Advance the pointer past the user guess.

        if(userGuess == actualNumber){
            sprintf((char *)ptr,"%s","win");
            sem_post((sem_t *) sem_ptr_msg);
            notWinning = 0;
            break;
        }
        else if(userGuess < actualNumber){
            sprintf((char *)ptr,"%s","low");
            sem_post((sem_t *) sem_ptr_msg);
            ptr = buf_start;
        }
        else if(userGuess > actualNumber){
            sprintf((char *)ptr,"%s","high");
            sem_post((sem_t *) sem_ptr_msg);
            ptr = buf_start;
        }
    }

    /* remove the shared memory segment */
    if (shm_unlink(name) == -1) {
        printf("Error removing %s\n", name);
        exit(-1);
    }
    return 0;
}

