
#include<stdlib.h>
#include<sys/wait.h>
#include<unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define CHILD_COUNT 3
#define MAX_WORD_LENGTH 50

typedef struct {
    char path_to_file[CHILD_COUNT][MAX_WORD_LENGTH];
    char word_to_find[CHILD_COUNT][MAX_WORD_LENGTH];
    int nr_of_occurrences[CHILD_COUNT];
} shared_data;

int count_word_occurrences(const char *filename, const char *word);
bool has_delimiter(char c);
char *trim_word(char *word);

int main() {

    srand(time(NULL));
    pid_t child[CHILD_COUNT];

    int fd, data_size = sizeof(shared_data);
    shared_data *data;

    fd = shm_open("/ex8", O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);
    ftruncate(fd, data_size);
    data = (shared_data *) mmap(NULL, data_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    //initialize shared memory area values
    strcpy(data->path_to_file[0], "text1.txt");
    strcpy(data->word_to_find[0], "monkeys");

    strcpy(data->path_to_file[1], "text.txt");
    strcpy(data->word_to_find[1], "capitalise");

    strcpy(data->path_to_file[2], "text2.txt");
    strcpy(data->word_to_find[2], "bird");

    for(int j = 0; j < CHILD_COUNT; j++) {
        if ((child[0] = fork()) == 0) {
            fd = shm_open("/ex8", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
            ftruncate(fd, data_size);
            data = (shared_data *) mmap(NULL, data_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

            int nr_of_occurrences = count_word_occurrences(data->path_to_file[j], data->word_to_find[j]);
            data->nr_of_occurrences[j] = nr_of_occurrences;

            munmap(data, data_size);
            close(fd);
            exit(1);
        }
    }

    for(int i = 0; i < CHILD_COUNT; i++){
        waitpid(child[i], NULL, 0);
    }

    for(int x = 0; x < CHILD_COUNT; x++) {
        printf("Word [%s] found %d times\n", data->word_to_find[x], data->nr_of_occurrences[x]);
    }

    munmap(data, data_size);
    close(fd);
    shm_unlink("/ex8");

    return 0;
}

char *trim_word(char *word){
    char  *new_word = malloc(strlen(word-1));

    if(has_delimiter(word[strlen(word) - 1])){

        for(int i = 0; i <= strlen(word) - 2; i++){
            new_word[i] = word[i];
        }
        return new_word;
    }
    else {
        return word;
    }


}

bool has_delimiter(char c) {
    return (c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == ',' || c == '.' || c == ';' || c == ':' || c == '!' || c == '?' || c == '(' || c == ')' || c == '[' || c == ']' || c == '{' || c == '}' || c == '<' || c == '>' || c == '\"' || c == '\'' || c == '/');
}

int count_word_occurrences(const char *filename, const char *word) {
    int count = 0;
    FILE *file = fopen(filename, "r");
    char *buffer = malloc(strlen(word) * sizeof(char));
    char *new_word = malloc(strlen(word) * sizeof(char));
    if (file == NULL) {
        perror("Error opening file");
        return -1;
    }

    while (fscanf(file, "%255s", buffer) != EOF) {
        new_word = trim_word(buffer);

        if (strncmp(new_word, word, strlen(word)) == 0) {
            count++;
        }
    }

    fclose(file);
    return count;
}


