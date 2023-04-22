
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

#define CHILD_COUNT 2

typedef struct {
    int number;
    char name[50];
    int courses[50];
} student;

int main() {

    srand(time(NULL));
    pid_t child[2];

    int fd, data_size = sizeof(student);
    student *data;

    fd = shm_open("/ex7", O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);
    ftruncate(fd, data_size);
    data = (student *) mmap(NULL, data_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    // Fill the array with random grades
    for (int i = 0; i < 50; i++) {
        data->courses[i] = rand() % 20;
    }

    data->number = 1200922;
    strcpy(data->name, "Francisco");

    if((child[0] = fork()) == 0 ){
        fd = shm_open("/ex7", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
        ftruncate(fd, data_size);
        data = (student *) mmap(NULL, data_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

        int max_grade = -1;

        for(int i = 0; i< 50; i++){
            if(data->courses[i] > max_grade){
                max_grade = data->courses[i];
            }
        }
        printf("max grade = %d\n", max_grade);
        munmap(data, data_size);
        close(fd);
        exit(1);
    }

    if((child[1] = fork()) == 0 ){
        fd = shm_open("/ex7", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
        ftruncate(fd, data_size);
        data = (student *) mmap(NULL, data_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

        int grade_sum = -1;

        for(int i = 0; i< 50; i++){
            grade_sum += data->courses[i];
        }

        printf("average grade = %f\n", (float )(grade_sum / 50));
        munmap(data, data_size);
        close(fd);
        exit(1);
    }



    for(int i = 0; i < CHILD_COUNT; i++){
        waitpid(child[i], NULL, 0);
    }

    /*
    for(int i = 0; i < 50; i++){
        printf("grade %d = %d\n", i, data->courses[i]);
    }
     */


    munmap(data, data_size);
    close(fd);
    shm_unlink("/ex7");

    return 0;
}


