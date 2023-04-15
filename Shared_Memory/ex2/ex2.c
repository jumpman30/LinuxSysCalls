#include<stdio.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<unistd.h>
#include<time.h>
#include <signal.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdbool.h>

typedef struct {
    int value2;
    int value1;
    bool SON;
    bool FATHER;
} shared_data;

int main() {

    int fd, data_size = sizeof(shared_data);
    shared_data *data;

    fd = shm_open("/ex2", O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);
    ftruncate(fd, data_size);
    data = (shared_data *) mmap(NULL, data_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    data->SON = false;
    data->FATHER = false;

    if(fork() == 0){

        fd = shm_open("/ex2", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
        ftruncate(fd, data_size);
        data = (shared_data *) mmap(NULL, data_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

            while(!data->SON);
            data->value1 = 1000;
            data->value2 = 2000;
            data->FATHER = true;

            munmap(data, data_size);
            close(fd);
            exit(1);
    }

    data->SON = true;

    while(!data->FATHER);

    printf("value: %d && %d\n", data->value1, data->value2);

    munmap(data, data_size);
    close(fd);
    shm_unlink("/ex2");
    return 0;
}