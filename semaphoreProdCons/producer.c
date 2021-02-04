#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <semaphore.h>

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
    //TODO: Add constant names of semaphore for protecting reading and writing to the guessed number buffer
    const char *semNameMsg = "semaphoreMsg";
    char userInput[50]; //String to collect the guessed number from the user
    char result[10]; //Message received from the producer telling whether the number is correct, high, or low
    int userGuess; //Number the user guessed converted to an integer
    int shm_fd; //Share memory file descriptor
    void *ptr; //Current location in your shared memory buffer. Moves around
    void *buf_start; //Use this to mark the beginning of your shared memory buffer
    sem_t *sem_ptr_msg;
    //TODO: Add pointers for read and write semaphores


    /* Create the shared memory segments */
    shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
    //TOOO: Initialize read and write semaphores
    sem_ptr_msg = sem_open(semNameMsg, O_CREAT, 0666);

    /* Configure size of shared memory segment */
    ftruncate(shm_fd, SIZE);

    /* now map the shared memory segment in the address space of the process */
    ptr = mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);


    if (ptr == MAP_FAILED) {
        printf("Map failed.\n");
        return -1;
    }

    //Test to see if the semaphore works
    int notWinning = 1;
    //Initialize your read and write semaphores protecting the shared integer
    sem_init((sem_t *) sem_ptr_msg, 1, 0);
    while (notWinning) {
        //Get a number from the user

        //Write the number to the shared buffer


        //Advance Pointer past the integer to an area where the message from the consumer can be writted


        //Wait for the consumer to signal the semaphore stating that the message is ready

        //Grab the message from the consumer process. Use an if statement to determine if the guess was high/low/correct.

    } //End notWinning loop
    return 0;
}