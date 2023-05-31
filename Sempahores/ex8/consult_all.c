#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>
#include <semaphore.h>
#include "record.h"

int main(void){
    int fd = shm_open("/shm", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);


    int size = sizeof(data);

    if(ftruncate(fd, size) == -1){
        perror("FTruncate error\n");
        exit(0);
    }

    data *shared_area = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    if(shared_area == NULL){
        printf("Erro no mmap\n");
        exit(0);
    }

    sem_t *sem;

    sem = sem_open("/sem_ex10", O_CREAT, 0644, 1);
    if(sem == SEM_FAILED){
        printf("Semaphore Error\n");
        return 0;
    }

    sem_wait(sem);

    if(shared_area->index == 0){
        printf("No records available.\n");
    }else{
        int i;
        for(i = 0; i < shared_area->index; i++){
            record *r = &(shared_area->entry[i]);
            printf("Record: %d\nNumber: %d  Name: %s  Address: %s\n", i+1, r->number, r->name, r->address);
            printf("\n");
        }
    }

    sem_post(sem);

    sem_unlink("/sem");

    munmap(shared_area, size);
    close(fd);

    return 0;
}