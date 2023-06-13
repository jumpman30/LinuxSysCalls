#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define SIZE 300
#define THREADS 5

typedef struct {
    int number;
    int notaG1;
    int notaG2;
    int notaG3;
    int notaFinal;
} Prova;

Prova provas[SIZE];

int provas_status[SIZE];

volatile int pos = 0;
volatile int neg = 0;

pthread_cond_t cond_prova;
pthread_cond_t cond_result;

pthread_mutex_t mutex_prova;
pthread_mutex_t mutex_result;

int random_number(int lower, int upper){
    return (rand() % (upper - lower + 1)) + lower;
}

void initialize_array(int* array, int size) {
    for (int i = 0; i < size; i++)
        *(array + i) = -1;
}

void* thread_populate_provas(void* arg){
    //Populates provas array with random values
    printf("Populating provas...\n");
    int i;
    for(i = 0; i < SIZE; i++) {
        provas[i].number = random_number(1111111, 9999999);
        provas[i].notaG1 = random_number(0, 100);
        provas[i].notaG2 = random_number(0, 100);
        provas[i].notaG3 = random_number(0, 100);

        //Sets status of prova as "registered" and signals computing threads
        provas_status[i] = 0;
        pthread_cond_signal(&cond_prova);
    }
    printf("Database complete with %d registers\n", i);
}

void* thread_compute_final_grade(void* arg){
    //Calculates final grades
    int index = 0;

    while(index != SIZE) {

        while(provas_status[index] == 1 && index < SIZE - 1)
            index++;
        if(provas_status[index] == -1) {
            pthread_mutex_lock(&mutex_prova);
            pthread_cond_wait(&cond_prova, &mutex_prova);
        }

        printf("Thread %d computing grade in index %d...\n", pthread_self(), index);
        provas[index].notaFinal = (provas[index].notaG1 + provas[index].notaG2 + provas[index].notaG3) / 3;
        printf("Final grade is %d\n", provas[index].notaFinal);
        pthread_mutex_unlock(&mutex_prova);

        provas_status[index] = 1;
        pthread_cond_signal(&cond_result);

        index++;
    }

    pthread_exit((void*)NULL);
}

void* thread_result(void* arg){
    int index = 0;

    while(index != SIZE) {

        while(provas_status[index] == 2 && index < SIZE - 1)
            index++;
        if(provas_status[index] == -1 || provas_status[index] == 0) {
            pthread_mutex_lock(&mutex_result);
            pthread_cond_wait(&cond_result, &mutex_result);
        }
        pthread_mutex_unlock(&mutex_result);

        printf("Thread %d adding grade to statistics...\n", pthread_self());
        if(provas[index].notaFinal >= 50)
            pos++;
        else
            neg++;

        provas_status[index] = 2;

        index++;
    }
    pthread_exit((void*)NULL);
}

int main() {
    pthread_mutex_init(&mutex_prova, NULL);
    pthread_mutex_init(&mutex_result, NULL);
    pthread_cond_init(&cond_prova, NULL);
    pthread_cond_init(&cond_result, NULL);

    pthread_t threads[THREADS];
    initialize_array(provas_status, SIZE);

    srand ( time(NULL) );

    if(pthread_create(&threads[3], NULL, thread_result, (void*)NULL))
        perror("Error creating thread.");
    if(pthread_create(&threads[4], NULL, thread_result, (void*)NULL))
        perror("Error creating thread.");

    if(pthread_create(&threads[1], NULL, thread_compute_final_grade, (void*)NULL))
        perror("Error creating thread.");
    if(pthread_create(&threads[2], NULL, thread_compute_final_grade, (void*)NULL))
        perror("Error creating thread.");

    if(pthread_create(&threads[0], NULL, thread_populate_provas, (void*)NULL))
        perror("Error creating thread.");

    printf("Terminating all threads...\n");
    for (int i = 0; i < THREADS; i++) {
        if (pthread_join(threads[i], NULL))
            perror("Failed to join thread");
    }

    pthread_mutex_destroy(&mutex_prova);
    pthread_mutex_destroy(&mutex_result);
    pthread_cond_destroy(&cond_prova);
    pthread_cond_destroy(&cond_result);

    printf("Percentages: Positive results %.2f%%  Negative results %.2f%%\n", ((float)pos/(float)SIZE)*100, ((float)neg/(float)SIZE)*100);

    return 0;
}