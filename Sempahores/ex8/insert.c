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
        perror("Ftruncate error\n");
        exit(0);
    }

    data *shared_are = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    if(shared_are == NULL){
        printf("Erro no mmap\n");
        exit(0);
    }

    sem_t *sem;

    sem = sem_open("/sem", O_CREAT, 0644, 1);

    sem_wait(sem);

    record *r = &(shared_are->entry[shared_are->index]);

    printf("Insert number:\n");

    scanf("%d", &(r->number));
    fflush(0);

    printf("Insert name:\n");

    char aux[50];
    scanf("%s", aux);
    strcpy(r->name, aux);
    fflush(0);

    printf("Insert address:\n");

    char aux2[100];
    scanf("%s", aux2);
    strcpy(r->address, aux2);
    fflush(0);

    shared_are->index++;

    sem_post(sem);

    sem_unlink("/sem");

    munmap(shared_are, size);
    close(fd);

    return 0;
}
