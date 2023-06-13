#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#define SIZE_OF_ARRAY 1000
#define SIZE_OF_CHUNK 200
#define NUMBER_OF_THREADS 5

typedef struct {
    int numberToSearch;
    int indexOfThread;
    int* array;
} Data;

void* printContents(void* arg) {
    Data* argumento = (Data*)arg;

    int numberToSearch = argumento->numberToSearch;
    int indexOfThread = argumento->indexOfThread;
    int* array = argumento->array;

    int start = indexOfThread * SIZE_OF_CHUNK;
    int end = (indexOfThread + 1) * SIZE_OF_CHUNK;

    int k;
    for (k = start; k < end; k++) {
        if (array[k] == numberToSearch) {
            printf("The number %d was found by thread number (from inside): %d\n", array[k], indexOfThread);
            fflush(stdout);
            pthread_exit((void*)indexOfThread);
        }
    }

    pthread_exit((void*)NULL);
}

int main(int argc, char* agrv[]) {

    pthread_t threads[NUMBER_OF_THREADS];

    Data threadData[NUMBER_OF_THREADS];

    int myArray[SIZE_OF_ARRAY];

    int numberToSearch;

    printf("Enter a number to search the array:\n");
    scanf("%d", &numberToSearch);
    printf("\n");
    fflush(stdout);

    int i;
    for (i = 0; i < SIZE_OF_ARRAY; i++) {
        myArray[i] = i;
    }

    int k;
    for (k = 0; k < NUMBER_OF_THREADS; k++) {
        threadData[k].array = myArray;
        threadData[k].indexOfThread = k;
        threadData[k].numberToSearch = numberToSearch;

        pthread_create(&threads[k], NULL, printContents, (void*)&threadData[k]);
    }

    int j;
    for (j = 0; j < NUMBER_OF_THREADS; j++) {
        int retorno;
        pthread_join(threads[j], (void*)&retorno);
        printf("A thread finished with the return value of: %d\n", retorno);
    }

    return EXIT_SUCCESS;
}