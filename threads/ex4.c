#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

#define MATRIX_SIZE 2
#define THREAD_POOL_SIZE 2

int matrix1[MATRIX_SIZE][MATRIX_SIZE] = {
        {2, 2},
        {2, 2}
};

int matrix2[MATRIX_SIZE][MATRIX_SIZE] = {
        {2, 2},
        {2, 2}
};

int matrix_result[MATRIX_SIZE][MATRIX_SIZE];
pthread_mutex_t mux;

void printMatrix(int x);

void* threadFunction(void* arg) {
    int target = *(int*)arg;
    for(int i = 0; i < MATRIX_SIZE; i++){
        int sum = 0;
        for(int j = 0; j<MATRIX_SIZE; j++){
            sum += matrix1[target][j] * matrix2[i][j];
        }
        pthread_mutex_lock(&mux);
        printf("final sum %d on thread %d...writing at [%d][%d]...\n", sum, pthread_self(), target, i);
        matrix_result[target][i] = sum;
        pthread_mutex_unlock(&mux);
    }
    return NULL;
}

int main() {

    pthread_t thread[THREAD_POOL_SIZE];
    pthread_mutex_init(&mux, NULL);

    for(int i=0; i<THREAD_POOL_SIZE; i++){
        int* threadNumber = malloc(sizeof(int));
        *threadNumber = i;
        pthread_create(&thread[i], NULL, threadFunction, threadNumber);
    }


    for(int i=0; i<THREAD_POOL_SIZE; i++){
        pthread_join(thread[i], NULL);
    }

    pthread_mutex_destroy(&mux);
    printMatrix(THREAD_POOL_SIZE);

    return 0;
}

void printMatrix(int x) {
    int i, j;

    printf("\nFINAL MATRIX \n");
    for (i = 0; i < x; i++) {
        for (j = 0; j < x; j++) {
            printf("%d ", matrix_result[i][j]);
        }
        printf("\n");
    }
}




