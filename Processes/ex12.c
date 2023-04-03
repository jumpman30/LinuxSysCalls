//
// Created by Signals Pereira on 02/03/2023.
//

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

int spawn_children(int n, pid_t *pid);

#define CHILD_COUNT 6


int main(void) {

    pid_t *pid;
    int status;

    pid = (pid_t *) calloc(CHILD_COUNT, sizeof(pid_t));

    // Check if memory allocation was successful
    if (pid == NULL) {
        printf("Memory allocation failed\n");
        return 0;
    }

    spawn_children(CHILD_COUNT, pid);

    for(int i = 0; i < CHILD_COUNT; i++){
        waitpid(pid[i], &status, 0);
        printf("Child %d exited with status %d\n", pid[i], WEXITSTATUS(status));
    }

    for(int i = 0; i < CHILD_COUNT; i++){
        waitpid(pid[i], &status, 0);
        printf("Child %d exited with status %d\n", pid[i], WEXITSTATUS(status));
    }

    free(pid);

}

int spawn_children(int n, pid_t *pid){

    for(int i = 0; i < n; i++){
        if((pid[i] = fork()) == 0){
            printf("I am child number %d with pid %d\n", i, getpid());
            exit(i * 2);
        }
    }

    return 0;
}


