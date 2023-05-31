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
    sem_t *canRead;
    sem_t *canWrite;
    sem_t *index_access;
    sem_t *x;
    sem_t *normal;
    sem_t *vip;
    sem_t *special;
    int fd, data_size = sizeof(shared_data_type);
    shared_data_type *shared_data;

    fd = shm_open("/pl4ex17", O_CREAT|O_RDWR, S_IRUSR|S_IWUSR);
    ftruncate(fd, data_size);
    shared_data = (shared_data_type*)mmap(NULL, data_size, PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);

    canRead = sem_open("sem_read", O_CREAT);
    canWrite = sem_open("sem_write", O_CREAT);
    index_access = sem_open("sem_access", O_CREAT);
    x = sem_open("sem_x", O_CREAT);
    normal = sem_open("sem_normal", O_CREAT);
    vip = sem_open("sem_vip", O_CREAT);
    special = sem_open("sem_special", O_CREAT);

    shared_data->free_seats = 2;
    shared_data->current = 1;
    shared_data->index = 1;
    shared_data->nr_vips = 0;
    shared_data->nr_normal = 0;
    shared_data->nr_specials = 0;

    while(shared_data->free_seats > 0){
        sem_wait(canWrite);
        if (shared_data->client_type == 1){
            shared_data->nr_vips += 1;
        } else if (shared_data->client_type == 2){
            shared_data->nr_specials += 1;
        } else {
            shared_data->nr_normal += 1;
        }
        shared_data->free_seats--;
        printf("Go in!\n");
        sem_post(canRead);
    }

    while(1){
        sem_wait(canWrite);
        if (shared_data->client_type == 1){
            if (shared_data->nr_normal > 0){
                shared_data->nr_normal-=1;
                shared_data->nr_vips+=1;
                sem_post(normal);
            } else if (shared_data->nr_specials > 0){
                shared_data->nr_specials-=1;
                shared_data->nr_vips+=1;
                sem_post(special);
            } else {
                printf("Full");
                shared_data->occupied = 1;
            }
        }
        if (shared_data->client_type == 2){
            if (shared_data->nr_normal > 0){
                shared_data->nr_normal-=1;
                shared_data->nr_specials+=1;
                sem_post(normal);
            } else {
                printf("FULL");
                shared_data->occupied = 1;
            }
        }
        if (shared_data->client_type== 3){
            printf("Full");
            shared_data->occupied = 1;
        }
        sem_post(canRead);
    }

    printf("Over...\n");

}