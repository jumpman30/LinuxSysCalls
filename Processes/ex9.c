//
// Created by Signals Pereira on 01/03/2023.
//

//
// Created by Signals Pereira on 28/02/2023.
//

//
// Created by Signals Pereira on 28/02/2023.
//
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define NUMBER_COUNT 100
#define CHILD_COUNT 10


int main(void) {

    int current = 1;
    pid_t pid[CHILD_COUNT];
    int status;

    for (int i = 0; i < CHILD_COUNT; i++) {

        if( (pid[i] = fork()) == 0){

            for(int j = 0; j < NUMBER_COUNT; j++){
                    printf("Child %d prints %d\n", getpid(), current);
                current++;
            }
            exit(0);
        }
        current += NUMBER_COUNT;
    }

    for(int i = 0; i < CHILD_COUNT; i++){
        waitpid(pid[i], &status, 0);
        printf("Child %d exited with status %d\n", pid[i], WEXITSTATUS(status));
    }

}


