//
// Created by Signals Pereira on 02/03/2023.
//
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

#define ARRAY_SIZE 1000
#define CHILD_COUNT 5
#define EXIT 0


int main(void) {

    int numbers[ARRAY_SIZE], current_index = 0;
    float result[ARRAY_SIZE];
    pid_t pid[CHILD_COUNT];
    int current = 0, exit_status, max_value = -1;
    time_t t;


    srand ((unsigned) time (&t));



    for(int j = 0; j < ARRAY_SIZE; j++){
        numbers[j] = rand() % 255;
    }

    for (int i = 0; i < CHILD_COUNT; i++) {

        if( (pid[i] = fork()) == 0){
            int current_max = -1;

            for(int j = current; j < ARRAY_SIZE / CHILD_COUNT; j++){
                if(numbers[j] > current_max)
                    current_max = numbers[j];
            }
            exit(current_max);
        }
        current += ARRAY_SIZE / CHILD_COUNT;
    }

    for(int i = 0; i < CHILD_COUNT; i++){
        waitpid(pid[i], &exit_status, 0);
        if(WEXITSTATUS(exit_status) > max_value)
            max_value = WEXITSTATUS(exit_status);
    }

    printf("Found maximum value %d\n", max_value);

    if(fork() == 0){
        for(int i = current_index; i < ARRAY_SIZE / 2; i++){
            result[i] = ((float) numbers[i] / (float) max_value) * 100;
        }
        exit(EXIT);
    }

    current_index+= (ARRAY_SIZE / 2);

    for(int i = current_index; i < ARRAY_SIZE; i++){
        result[i] = ((float) numbers[i] / (float) max_value) * 100;
    }

    for(int i = current_index; i < ARRAY_SIZE; i++){
        printf("result[%d] = %f\n", i, result[i]);
    }
}


