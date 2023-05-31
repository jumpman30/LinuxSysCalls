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
#include <string.h>

#define BUFFER 10
#define OPERATIONS 30


typedef struct {
    int buffer[BUFFER];
    int head;
    int tail;
    int size;
} circular_buffer;



int spawn_childs(int n) {

    int i;

    for (i = 0; i < n; i++) {
        pid_t p = fork();
        if (p == 0) {
            return i+1;
        } else if (p > 0) {
            wait(NULL);
        }
    }
    return 0;
}



circular_buffer *start_buffer() {
    int fd, ft, data_size = sizeof(circular_buffer);
    circular_buffer *shared;

    shm_unlink ("/shm");

    fd = shm_open("/shm", O_CREAT|O_EXCL|O_RDWR, S_IRUSR|S_IWUSR);

    ftruncate (fd, data_size);

    shared = (circular_buffer*)mmap(NULL, data_size, PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
    shared->size = BUFFER;
    shared->head = 0;
    shared->tail = 0;

    return shared;
}



void write_value_buffer(circular_buffer *shared, int value) {
    shared->buffer[shared->head] = value;
    shared->head = (shared->head + 1) % shared->size;
}

void read_value_buffer(circular_buffer *shared, int *value) {
    *value = shared->buffer[shared->tail];
    shared->tail = (shared->tail + 1) % shared->size;
}



int check_full_buffer(circular_buffer *shared) {
    if ((shared->head + 1) % shared->size == shared->tail) {
        return 1;
    }
    return 0;
}

int check_empty_buffer(circular_buffer *shared) {
    if (shared->head == shared->tail) {
        return 1;
    }
    return 0;
}



int main(void) {

    sem_t *sem_shared_area;
    sem_shared_area = sem_open("sem_shared_area", O_CREAT | O_EXCL, 0644, 1);

    sem_t *sem_full;
    sem_full = sem_open("sem_full", O_CREAT | O_EXCL, 0644, 1);

    sem_t *sem_empty;
    sem_empty = sem_open("sem_empty", O_CREAT | O_EXCL, 0644, 1);


    circular_buffer *shared = start_buffer();
    int i, j = 0, k = 0;

    pid_t p1 = fork();

    if (p1 == 0) {
        while(j < 3) {
            for (i = 0; i < 10; i++) {
                sem_wait(sem_empty);
                sem_wait(sem_shared_area);

                int value_read;
                read_value_buffer(shared, &value_read);

                sem_post(sem_shared_area);
                sem_post(sem_full);
            }
            j++;
        }
    }
    if (p1 > 0) {
        while(k < 3) {
            for (i = 0; i < 10; i++) {
                sem_wait(sem_full);
                sem_wait(sem_shared_area);

                write_value_buffer(shared, i+1);

                sem_post(sem_shared_area);
                sem_post(sem_empty);
            }
            k++;
        }
        wait(NULL);

        if (munmap(shared, 400) < 0) {
            exit(1);
        }
    }
    return 0;
}