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
    const char *semNameNumWrite = "semaphoreNumWrite";
    const char *semNameNumRead = "semaphoreNumRead";
    const char *semNameMsg = "semaphoreMsg";
    //const char *message0 = "Studying ";
    //const char *message1 = "Operating Systems ";
    //const char *message2 = "is fun!";
    char userInput[50];
    char result[10];
    int userGuess;
    int shm_fd;
    void *ptr;
    void *buf_start;
    sem_t *sem_ptr_num_write; //Ready to write to guessed number buffer
    sem_t *sem_ptr_msg;
    sem_t *sem_ptr_num_read; //Ready to read from guessed number buffer


    /* Create the shared memory segments */
    shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
    sem_ptr_num_write = sem_open(semNameNumWrite, O_CREAT, 0666);
    sem_ptr_num_read = sem_open(semNameNumRead, O_CREAT, 0666);
    sem_ptr_msg = sem_open(semNameMsg, O_CREAT, 0666);

    /* Configure size of shared memory segment */
    ftruncate(shm_fd, SIZE);

    /* now map the shared memory segment in the address space of the process */
    ptr = mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    buf_start = ptr; //Remember where the start of the buffer is

    if (ptr == MAP_FAILED) {
        printf("Map failed.\n");
        return -1;
    }

    //Test to see if the semaphore works
    int notWinning = 1;
    sem_init((sem_t *) sem_ptr_num_write, 1, 1); //Ready to write
    sem_init((sem_t *) sem_ptr_num_read, 1, 0); //Not ready to read
    sem_init((sem_t *) sem_ptr_msg, 1, 0);
    while (notWinning) {
        printf("Guess a number between 1 and 15\n");
        fgets(userInput,50,stdin);
        userGuess = atoi(userInput);

        //CS while writing to the number guessed
        sem_wait((sem_t *) sem_ptr_num_write);
        sprintf((char *) ptr,"%d",userGuess);
        sem_post((sem_t *) sem_ptr_num_read); //Ready for reading, not ready for writing Read = 1, Write = 0

        //Advance Pointer past the integer
        ptr += sizeof((char) userGuess + 1);

        //sleep(1); //Need to fix this

        sem_wait((sem_t *) sem_ptr_msg); //Wait for the consumer to say it's done

        sprintf(result,"%s",(char *) ptr);
        if(strcmp(result,"win")==0){
            notWinning = 0;
            printf("You won the game!\n");
            break;
        }
        else if(strcmp(result,"low") == 0){
            ptr = buf_start;
            printf("Your guess was too low.\n");

        }
        else if(strcmp(result,"high")==0){
            ptr = buf_start;
            printf("Your guess was too high.\n");

        }
        else{
            printf("There was an error reading from the shared buffer\n");
        }
    }
    printf("Producer exit!\n");


    return 0;
}