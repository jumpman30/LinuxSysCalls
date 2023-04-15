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

#define FILE_NAME "/ex1";

typedef struct {
    char name[30];
    int number;
    char address[30];
} student_data;

int main() {

    int fd, data_size = sizeof(student_data);
    student_data *student;

    fd = shm_open("/ex1", O_RDWR, S_IRUSR | S_IWUSR);
    ftruncate(fd, data_size);
    student = (student_data *) mmap(NULL, data_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    printf("student number: %d\n", student->number);


    munmap(student, data_size);
    close(fd);
    shm_unlink("/ex1");

    return 0;
}