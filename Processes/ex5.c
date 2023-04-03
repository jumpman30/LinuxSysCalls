//
// Created by Signals Pereira on 28/02/2023.
//
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define ARRAY_SIZE 2


int main(void) {

    int pid[ARRAY_SIZE];

    for(int i = 0; i < ARRAY_SIZE; i++){
        pid[i] = fork();
        if(pid[i] == 0){
            printf("Child %d executing\n", (int)getpid());
            int child_var = i + 1;
            sleep(child_var);
            exit(child_var);
        }
    }

    for(int i= 0; i < ARRAY_SIZE; i++){
        int status;
        waitpid(pid[i], &status, 0);
        if(WIFEXITED(status))
            printf("Child %d exited with status %d\n", pid[i], WEXITSTATUS(status));
    }

}