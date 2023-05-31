#include<stdlib.h>
#include<sys/wait.h>
#include<unistd.h>
#include <string.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdio.h>
#include <time.h>
#include <semaphore.h>

#define ARRAY_SIZE 200
#define NUMBER_COUNT 1
#define CHILD_COUNT 8
#define SEMAPHORE 8

typedef struct {
    long read_start_position;
} shared_data;


int* read_numbers(const char *filename, int number_count);
void write_numbers(int *numbers, char *filename);

int main() {

    sem_t *semaphores_read[SEMAPHORE];
    sem_t *semaphores_write[SEMAPHORE];

    for(int i = 0; i < SEMAPHORE; i++){
        char sem_name[10];
        sprintf(sem_name, "%d", i);
        semaphores_read[i] = sem_open(sem_name, O_CREAT | O_EXCL, 0644, 0);
        semaphores_write[i] = sem_open(sem_name, O_CREAT | O_EXCL, 0644, 0);
    }

    remove("Output.txt");
    pid_t child[CHILD_COUNT];
    int fd, data_size = sizeof(shared_data);
    shared_data *data;

    fd = shm_open("/ex1", O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);
    ftruncate(fd, data_size);
    data = (shared_data *) mmap(NULL, data_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    data->read_start_position = 0;


    sem_post(semaphores_read[0]);

    for(int j = 0; j < CHILD_COUNT; j++) {
        if ((child[j] = fork()) == 0) {
            int* numbers = malloc(NUMBER_COUNT * sizeof(int));


            //todo: Critical section
            sem_wait(semaphores_read[j]);
            numbers = read_numbers("Numbers.txt" ,NUMBER_COUNT);
            sem_post(semaphores_read[j+1]);
            //todo: End critical section

            sem_post(semaphores_read[0]);
            //todo: Critical section
            sem_wait(semaphores_write[j]);
            write_numbers(numbers, "Output.txt");
            sem_post(semaphores_write[j+1]);
            //todo: End critical section

            exit(1);
        }
    }

    for(int i = 0; i < CHILD_COUNT; i++){
        waitpid(child[i], NULL, 0);
    }

    munmap(data, data_size);
    sem_unlink("sem_read");
    shm_unlink("/ex1");
}


int* read_numbers(const char *filename, int number_count) {
    FILE* file = fopen(filename, "r");
    int* numbers = malloc(ARRAY_SIZE * sizeof(int));

    int fd, data_size = sizeof(shared_data);
    shared_data *data;
    long next_start_position;


    fd = shm_open("/ex1", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    ftruncate(fd, data_size);
    data = (shared_data *) mmap(NULL, data_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    if (file == NULL) {
        printf("Error opening file: %s\n", filename);
        return NULL;
    }

    fseek(file, data->read_start_position, SEEK_SET);

    int num, i = 0;

    while ( i < number_count && fscanf(file, "%d", &num) == 1) {
        numbers[i] = num;
        i++;
    }

    data->read_start_position = ftell(file) + 1;
    munmap(data, data_size);
    fclose(file); // Close the file
    return numbers;
}

void write_numbers(int *numbers, char *filename){

    //gives control to attach data instead of overriding the file
    int file = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (file == -1) {
        printf("Error opening file!\n");
        return;
    }

    for (int i = 0; i < NUMBER_COUNT ; i++) {
        dprintf(file, "%d\n", numbers[i]);
    }

    close(file);
}


