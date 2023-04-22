
#include<stdlib.h>
#include<sys/wait.h>
#include<unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ARRAY_SIZE 1000
#define CHILD_COUNT 10

typedef struct {
    int max[100];
} shared_data;

int main() {
    int random_numbers[ARRAY_SIZE];
    pid_t pid[ARRAY_SIZE / 10];
    srand(time(NULL));

    // Fill the array with random numbers
    for (int i = 0; i < ARRAY_SIZE; i++) {
        random_numbers[i] = rand() % 100; // Generate a random number between 0 and 99
    }

    int fd, data_size = sizeof(shared_data);
    shared_data *data;

    fd = shm_open("/ex6", O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);
    ftruncate(fd, data_size);
    data = (shared_data *) mmap(NULL, data_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    for(int i = 0; i < CHILD_COUNT; i++){
        if((pid[i] = fork()) == 0 ){

            fd = shm_open("/ex6", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
            ftruncate(fd, data_size);
            data = (shared_data *) mmap(NULL, data_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

            int local_max = -1;
            int start = i * 100;
            for(int j = start; j < start + 100; j ++ ){
                if(random_numbers[j] > local_max){
                    local_max = random_numbers[j];
                }
            }

            data->max[i] = local_max;

            munmap(data, data_size);
            close(fd);
            exit(1);
        }
    }


    for(int i = 0; i < CHILD_COUNT; i++){
        waitpid(pid[i], NULL, 0);
    }

    int global_max = -1;
    for(int i = 0; i < 10; i++){
        printf("Position %d = %d\n", i, data->max[i]);

        if(data->max[i] > global_max){
            global_max = data->max[i];
        }
    }
    printf("\n");

    printf("Global max = %d\n", global_max);
    munmap(data, data_size);
    close(fd);
    shm_unlink("/ex6");

    return 0;
}
