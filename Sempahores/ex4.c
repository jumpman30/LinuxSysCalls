#include<stdlib.h>
#include<sys/wait.h>
#include<unistd.h>
#include <string.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdio.h>
#include <time.h>
#include <semaphore.h>

#define SEM_CHILD_1 0
#define SEM_CHILD_2 2
#define SEM_PARENT 1
#define SEMAPHORE 3
#define CHILD_COUNT 2

void unlink_sem();

int main() {

    sem_t *semaphores[SEMAPHORE];
    pid_t child[CHILD_COUNT];

    for(int i = 0; i < SEMAPHORE; i++){
        char sem_name[10];
        sprintf(sem_name, "%d", i);
        semaphores[i] = sem_open(sem_name, O_CREAT | O_EXCL, 0644, 0);
    }

    //unlocking first child
    sem_post(semaphores[SEM_CHILD_1]);

    if ( (child[0] = fork()) == 0 ) {

        //TODO: Critical zone
        sem_wait(semaphores[SEM_CHILD_1]);
        printf("Child 1\n");
        sem_post(semaphores[SEM_PARENT]);
        //TODO: End Critical zone

        exit(1);
    }

    if((child[1] = fork()) == 0){
        //TODO: Critical zone
        sem_wait(semaphores[SEM_CHILD_2]);
        printf("Child 2\n");
        //TODO: End Critical zone

        exit(1);
    }

    //TODO: Critical zone
    sem_wait(semaphores[SEM_PARENT]);
    printf("Parent here\n");
    sem_post(semaphores[SEM_CHILD_2]);

    for(int i = 0; i < CHILD_COUNT; i++){
        waitpid(child[i], NULL, 0);
    }


    unlink_sem();

}


void unlink_sem(){
    for(int i = 0; i < SEMAPHORE; i++){
        char sem_name[10];
        sprintf(sem_name, "%d", i);
        sem_unlink(sem_name);
    }
}




