#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

#define ARRAY_SIZE 5


typedef struct{
    char name[30];
    int number;
    int grade;
}shared_data;

int getRandomNumber(int x);


void* threadFunction(void* arg) {
    shared_data *sharedVariable = (shared_data*)arg;
    printf("Thread: %d\n", pthread_self());
    printf("Number: %d\n", sharedVariable->number);
    printf("Grade: %d\n", sharedVariable->grade);
    return NULL;
}

int main() {
    shared_data *shared_array = malloc(ARRAY_SIZE * sizeof(shared_data));

    pthread_t thread[ARRAY_SIZE];

    for(int i=0; i<ARRAY_SIZE; i++){

        strcpy(shared_array[i].name, "Francisco");
        shared_array[i].number = getRandomNumber(100);
        shared_array[i].grade = getRandomNumber(20);
    }

    for(int i=0; i<ARRAY_SIZE; i++){
        pthread_create(&thread[i], NULL, threadFunction, (void*)&shared_array[i]);
    }


    for(int i=0; i<ARRAY_SIZE; i++){
        pthread_join(thread[i], NULL);
    }

    return 0;
}


int getRandomNumber(int X) {
    srand(time(NULL));

    int randomNumber = rand() % X;
    return randomNumber;
}

