#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define FILTERING_THREADS 3
#define COMPUTING_THREADS 3
#define PRODUCTS 5
#define HYPERMARKETS 3
#define SIZE 100

typedef struct {
    int id_h;
    int id_p;
    int p;
} Product;

typedef struct {
    int id_h;
    int cost;
} LowestCost;

Product vec[SIZE];
LowestCost lowestCost;

Product vec1[SIZE];
volatile int vec1_products = 0;

Product vec2[SIZE];
volatile int vec2_products = 0;

Product vec3[SIZE];
volatile int vec3_products = 0;


pthread_mutex_t mutex_vec1;
pthread_mutex_t mutex_vec2;
pthread_mutex_t mutex_vec3;
pthread_mutex_t mutex_computing;
pthread_mutex_t mutex_cost;

pthread_cond_t cond_filtering;


int random_number(int lower, int upper){
    return (rand() % (upper - lower + 1)) + lower;
}

void populate_database() {
    int i;
    for (i = 0; i < SIZE; i++) {
        vec[i].id_h = random_number(1, HYPERMARKETS);
        vec[i].id_p = random_number(1, PRODUCTS);
        vec[i].p = random_number(1, 50);
    }
}

void initialize_array(int* array, int size) {
    for (int i = 0; i < size; i++)
        *(array + i) = 0;
}

void copy_array(Product *a,Product *b,int n) {
    for(int i = 0; i < n; i++)
        b[i] = a[i];
}

void* thread_filtering(void* arg){
    int index = *(int*)arg;
    int end = index + SIZE/FILTERING_THREADS;
    printf("Thread analysing database from %d to %d...\n", index, end);

    for (int i = index; i < end; i++) {
        int id_h = vec[i].id_h;

        //todo: copy the array at the beginning and use one mutex instead of three
        if(id_h == 1) {
            pthread_mutex_lock(&mutex_vec1);
            vec1[vec1_products].id_h = vec[i].id_h;
            vec1[vec1_products].id_p = vec[i].id_p;
            vec1[vec1_products].p = vec[i].p;
            vec1_products++;
            pthread_mutex_unlock(&mutex_vec1);
        } else if(id_h == 2) {
            pthread_mutex_lock(&mutex_vec2);
            vec2[vec2_products].id_h = vec[i].id_h;
            vec2[vec2_products].id_p = vec[i].id_p;
            vec2[vec2_products].p = vec[i].p;
            vec2_products++;
            pthread_mutex_unlock(&mutex_vec2);
        } else if(id_h == 3) {
            pthread_mutex_lock(&mutex_vec3);
            vec3[vec3_products].id_h = vec[i].id_h;
            vec3[vec3_products].id_p = vec[i].id_p;
            vec3[vec3_products].p = vec[i].p;
            vec3_products++;
            pthread_mutex_unlock(&mutex_vec3);
        }
        //todo: signalize that computing threads can start
    }
    pthread_exit((void*)NULL);
}

void* thread_computing(void* arg){
    int id_h = *(int*)arg;
    printf("Booted up thread for hypermarket %d\n", id_h);
    //Defines which hypermarket this thread is dedicated to
    Product vec_h[SIZE];
    int vec_h_size;
    if (id_h == 1) {
        copy_array(vec1, vec_h, vec1_products);
        vec_h_size = vec1_products;
    } else if (id_h == 2) {
        copy_array(vec2, vec_h, vec2_products);
        vec_h_size = vec2_products;
    } else if (id_h == 3) {
        copy_array(vec3, vec_h, vec3_products);
        vec_h_size = vec3_products;
    }
    printf("Hypermarket assigned\n");

    //Waits for filtering process
    pthread_mutex_lock(&mutex_computing);
    printf("Waiting on filtering threads...\n");
    pthread_cond_wait(&cond_filtering, &mutex_computing);
    pthread_mutex_unlock(&mutex_computing);
    printf("Ready to compute!\n");

    int avg[PRODUCTS];
    int qty[PRODUCTS];
    initialize_array(avg, PRODUCTS);
    initialize_array(qty, PRODUCTS);

    for (int i = 0; i < vec_h_size; i++) {
        int prod_index = vec_h[i].id_p - 1;
        avg[prod_index] = avg[prod_index] + vec_h[i].p;
        qty[prod_index] = qty[prod_index] + 1;
    }
    for (int j = 0; j < PRODUCTS; j++) {
        avg[j] = avg[j]/qty[j];
    }

    int cost = 0;
    for (int k = 0; k < PRODUCTS; k++)
        cost = cost + avg[k];
    printf("Cost of products on hypermarket %d is %d\n", id_h, cost);

    pthread_mutex_lock(&mutex_cost);
    if(lowestCost.cost == -1 || lowestCost.cost > cost) {
        lowestCost.id_h = id_h;
        lowestCost.cost = cost;
    }
    pthread_mutex_unlock(&mutex_cost);

    printf("Thread finished computing\n");
    pthread_exit((void*)NULL);
}

int main() {

    pthread_mutex_init(&mutex_vec1, NULL);
    pthread_mutex_init(&mutex_vec2, NULL);
    pthread_mutex_init(&mutex_vec3, NULL);
    pthread_mutex_init(&mutex_cost, NULL);
    pthread_mutex_init(&mutex_computing, NULL);
    pthread_cond_init(&cond_filtering, NULL);

    lowestCost.id_h = -1;
    lowestCost.cost = -1;

    srand ( time(NULL) );
    populate_database();

    pthread_t filtering_threads[FILTERING_THREADS];
    int args_f[FILTERING_THREADS];

    for (int i = 0; i < FILTERING_THREADS; i++){
        args_f[i] = i * SIZE/FILTERING_THREADS;

        if(pthread_create(&filtering_threads[i], NULL, thread_filtering, (void*)&args_f[i]))
            perror("Error creating thread.");
    }

    pthread_t computing_threads[COMPUTING_THREADS];
    int args_c[COMPUTING_THREADS];

    for (int i = 0; i < COMPUTING_THREADS; i++){
        args_c[i] = i + 1;

        if(pthread_create(&computing_threads[i], NULL, thread_computing, (void*)&args_c[i]))
            perror("Error creating thread.");
    }


    for (int i = 0; i < FILTERING_THREADS; i++) {
        if (pthread_join(filtering_threads[i], NULL))
            perror("Failed to join thread");
    }
    printf("Filtering concluded\n");
    pthread_cond_broadcast(&cond_filtering);

    for (int i = 0; i < COMPUTING_THREADS; i++)
        if (pthread_join(computing_threads[i], NULL))
            perror("Failed to join thread");
    printf("Computing concluded\n");


    pthread_mutex_destroy(&mutex_vec1);
    pthread_mutex_destroy(&mutex_vec2);
    pthread_mutex_destroy(&mutex_vec3);
    pthread_mutex_destroy(&mutex_cost);
    pthread_mutex_destroy(&mutex_computing);
    pthread_cond_destroy(&cond_filtering);

    printf("Lowest Cost: ");
    printf("Hypermarket %d    Cost = %d\n", lowestCost.id_h, lowestCost.cost);

    return 0;
}