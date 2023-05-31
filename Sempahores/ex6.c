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

typedef struct{
    int diff;
} shared_data_type;


int main(void) {

    sem_t *sem1;
    sem_t *sem2;


    if ((sem1 = sem_open("sem1", O_CREAT | O_EXCL, 0644, 1)) == SEM_FAILED) {
        perror("Error in sem_open()");
        exit(1);
    }
    if ((sem2 = sem_open("sem2", O_CREAT | O_EXCL, 0644, 0)) == SEM_FAILED) {
        perror("Error in sem_open()");
        exit(1);
    }

    int fd, r, i = 0, data_size = sizeof(shared_data_type);
    shared_data_type *shared_data;

    fd = shm_open("/ex6", O_CREAT|O_RDWR, S_IRUSR|S_IWUSR);
    if (fd == -1){
        printf("Error opening shared memory area\n");
        return 1;
    }

    if (ftruncate (fd, data_size) == -1){
        printf("Error allocating memory on shared area\n");
        return 1;
    }

    shared_data = (shared_data_type*)mmap(NULL, data_size, PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
    shared_data->diff = 0;
    pid_t child = fork();

    if (child > 0)
    {
        while(i < 5){
            sem_wait(sem1);
            while (shared_data->diff < 1){
                printf("S\n");
                shared_data->diff += 1;
            }

            i++;
            sem_post(sem2);
        }

    } else {

        while (i < 5){
            sem_wait(sem2);
            while (shared_data->diff > -1){
                printf("C\n");
                shared_data->diff -= 1;
            }
            i++;
            sem_post(sem1);
        }
        exit(0);
    }

    wait(NULL);

    r = munmap(shared_data, data_size);
    if (r == -1){
        exit(1);
    }
    r = close(fd);
    if (r == -1){
        exit(1);
    }

    sem_unlink("sem1");
    sem_unlink("sem2");
    return 0;
}