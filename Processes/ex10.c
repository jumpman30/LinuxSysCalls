//
// Created by Signals Pereira on 02/03/2023.
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
#include <time.h>

#define ARRAY_SIZE 2000
#define CHILD_COUNT 10
#define ERROR_EXIT 255

int main(void) {

    int numbers[ARRAY_SIZE];
    pid_t pid[CHILD_COUNT];
    int current = 0, target_number, exit_status;
    time_t t;


    srand ((unsigned) time (&t));

    target_number = rand() % 10;

    printf("Target number: %d\n", target_number);

    for(int j = 0; j < ARRAY_SIZE; j++){
        numbers[j] = rand() % 10;
    }

    for (int i = 0; i < CHILD_COUNT; i++) {
        if( (pid[i] = fork()) == 0){
            for(int j = current; j < ARRAY_SIZE / CHILD_COUNT; j++){
                if(numbers[j] == target_number)
                    exit(j - (ARRAY_SIZE / CHILD_COUNT));
            }
            exit(ERROR_EXIT);
        }
        current += ARRAY_SIZE / CHILD_COUNT;
    }

    for(int i = 0; i < CHILD_COUNT; i++){
        waitpid(pid[i], &exit_status, 0);
        printf("Child %d exited with status %d\n", pid[i], WEXITSTATUS(exit_status));
    }


}


