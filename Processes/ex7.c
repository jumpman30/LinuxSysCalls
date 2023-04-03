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

#define ARRAY_SIZE 10


int main(void) {

    int numbers[ARRAY_SIZE];
    int target_number;
    time_t t;

    srand ((unsigned) time (&t));
    for(int j = 0; j < ARRAY_SIZE; j++){
        numbers[j] = rand() % 10;
    }

    target_number = rand() % 10;

    printf("Target number: %d\n", target_number);

    //executed by parent process in the first half of the array

    pid_t pid = fork();
    int parent_counter = 0;
    if(pid > 0) {
        for (int i = 0; i < ARRAY_SIZE / 2; i++) {
            if (numbers[i] == target_number)
                parent_counter++;
        }
    }
    else {
        int child_counter = 0;
        for(int i = ARRAY_SIZE / 2; i < ARRAY_SIZE; i++){
            if (numbers[i] == target_number)
                child_counter++;
        }
        exit(6);
        }


    int child_exit_status;
    waitpid(pid, &child_exit_status, 0);

    printf("Parent counter: %d\n", parent_counter);
    //child exit status consists on the last 8 significant bits of the status argument
    printf("Child counter: %d\n", WEXITSTATUS(child_exit_status));

    }


