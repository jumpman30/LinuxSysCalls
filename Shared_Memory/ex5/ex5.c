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
    int number;
} shared_data;

volatile sig_atomic_t can_access_shared_data = false;

void handle_USRR1(int signo, siginfo_t *siginfo, void *context) {
    can_access_shared_data = true;
}

int main() {

    //signal logic--------------------------
    struct sigaction act;
    pid_t child;

    memset(&act, 0, sizeof(struct sigaction));
    sigfillset(&act.sa_mask);
    sigdelset(&act.sa_mask, SIGUSR1);

    act.sa_sigaction = handle_USRR1;
    act.sa_flags = SA_SIGINFO;
    sigaction(SIGUSR1, &act, NULL);
    //------------------------------

    int fd, data_size = sizeof(shared_data);
    shared_data *data;

    fd = shm_open("/ex5", O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);
    ftruncate(fd, data_size);
    data = (shared_data *) mmap(NULL, data_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    if( (child = fork() ) == 0) {

        fd = shm_open("/ex5", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
        ftruncate(fd, data_size);
        data = (shared_data *) mmap(NULL, data_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

        for(int i = 0; i < 1000000; i++){
            while (!can_access_shared_data) pause;
            data->number++;
            can_access_shared_data = false;
        }

        printf("Child process received the signal\n");

        munmap(data, data_size);
        close(fd);
        exit(1);
    }

    data->number = 0;

    for(int i = 0; i < 1000000; i++) {
        sleep(2);
        kill(child, SIGUSR1);
        data->number--;
    }

    waitpid(child, NULL, 0);

    printf("Final value: %d\n", data->number);
    munmap(data, data_size);
    close(fd);
    shm_unlink("/ex5");
    return 0;
}