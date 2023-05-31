#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <semaphore.h>


typedef struct {
    int nr_tickets;
    int index;
    int current;
    int nr_vips;
    int nr_normal;
    int nr_specials;
    int client_type;
    int occupied;
    int free_seats;
} shared_data_type;

int main(void) {
    sem_t *index_access;
    sem_t *canRead;
    sem_t *canWrite;
    sem_t *x;
    sem_t *normal;
    int write_index, attended = 0;
    int fd, data_size = sizeof(shared_data_type);
    shared_data_type *shared_data;

    fd = shm_open("/shm", O_RDWR, S_IRUSR|S_IWUSR);
    ftruncate(fd, data_size);
    shared_data = (shared_data_type*)mmap(NULL, data_size, PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);

    canRead = sem_open("sem_read", O_CREAT);
    canWrite = sem_open("sem_write", O_CREAT);
    index_access = sem_open("sem_access", O_CREAT);
    x = sem_open("sem_x", O_CREAT);
    normal = sem_open("sem_normal", O_CREAT);

    sem_wait(index_access);
    write_index = shared_data->index;
    shared_data->index++;
    sem_post(index_access);

    while (!attended){
        sem_wait(x);
        if(write_index == shared_data->current){
            attended = 1;
        } else {
            sem_post(x);
        }
    }
    shared_data->client_type = 3;
    sem_post(canWrite);
    sem_wait(canRead);
    if (shared_data->occupied == 1){
        printf("Full");
        shared_data->occupied = 0;
        sem_post(x);
        return 0;
    }
    shared_data->current++;
    sem_post(x);
    printf("Watching...\n");
    sleep(1);
    sem_wait(normal);
    printf("Going out...\n");
}