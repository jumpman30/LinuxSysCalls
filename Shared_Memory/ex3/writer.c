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

///Implemented already using active waiting...

typedef struct {
    int values[10];
    bool WRITER;
    bool READER;
} shared_data;

int main() {

    int fd, data_size = sizeof(shared_data);
    shared_data *data;

    fd = shm_open("/ex3", O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);
    ftruncate(fd, data_size);
    data = (shared_data *) mmap(NULL, data_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    data->READER = false;
    data->WRITER = true;

    printf("Writing values... \n");

    for(int i = 0; i < 10; i++){
        data->values[i] = i;
    }
    data->READER = true;
    data->WRITER = false;

    while(!data->WRITER);

    printf("Finishing...");

    munmap(data, data_size);
    close(fd);
    shm_unlink("/ex3");

    return 0;
}