#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

#define DATABASE_SIZE 8000
#define THREAD_POOL_SIZE 16
#define LOTTERY_MAX 49
#define LOTTERY_PICKS 5
#define INDEX_BASE 8000/16

int database[DATABASE_SIZE][LOTTERY_PICKS];
int statistic[LOTTERY_MAX];
pthread_mutex_t mux;

void populate_database();
void initialize_statistics();
int generateRandomNumber();
void print_statistics();

void* threadFunction(void* arg) {
    int final_index = *(int*)arg * INDEX_BASE;

    for(int i=final_index-INDEX_BASE; i<final_index; i++ ){
       for(int j=0; j<LOTTERY_PICKS; j++){
           //by accessing to the database and statistic array at the same time
           //there is no need to use two mutexes
           pthread_mutex_lock(&mux);
           statistic[database[i][j]]++;
           pthread_mutex_unlock(&mux);
       }
    }
    pthread_exit((void *)NULL);
}

int main() {

    pthread_t thread[THREAD_POOL_SIZE];
    pthread_mutex_init(&mux, NULL);

    populate_database();
    initialize_statistics();

    for(int i=0; i<THREAD_POOL_SIZE; i++){
        int* threadNumber = malloc(sizeof(int));
        *threadNumber = i;
        pthread_create(&thread[i], NULL, threadFunction, threadNumber);
    }


    for(int i=0; i<THREAD_POOL_SIZE; i++){
        pthread_join(thread[i], NULL);
    }

    pthread_mutex_destroy(&mux);
    print_statistics();


    return 0;
}

void populate_database(){
    for(int i=0; i<DATABASE_SIZE;i++){
        for(int j=0; j<LOTTERY_PICKS; j++){
            database[i][j]=generateRandomNumber();
        }
    }
}

int generateRandomNumber() {
    srand(time(NULL));
    int randomNumber = (rand() % 49) + 1;
    return randomNumber;
}

void initialize_statistics(){
    for(int i=0; i<LOTTERY_MAX; i++){
        statistic[i]=0;
    }
}

void print_statistics(){
    for(int i=0; i<LOTTERY_MAX; i++){
        printf("Number %d was choosen %d times\n", i, statistic[i]);
    }
}




