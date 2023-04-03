//
// Created by Signals Pereira on 28/02/2023.
//
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

int main()
{
    pid_t p;

    if (fork() == 0) {
        //executed by child process 1
        printf("PID = %d\n", getpid());
        exit(0);
    }

    if ((p=fork()) == 0) {
        //executed by 2nd child processes
        printf("PID = %d\n", getpid());
        exit(0);
    }

    printf("Parent PID = %d\n", getpid());

    printf("Waiting... (for PID=%d)\n",p);
    //waits for child2
    waitpid(p, NULL, 0);

    printf("Enter Loop...\n");
    while (1);
}

//1.Only parent process runs in endless loop, so only the parent processes is listed
//child processes are exited after they printed their PID
