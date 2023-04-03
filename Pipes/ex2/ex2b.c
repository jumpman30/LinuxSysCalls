#include<stdio.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<unistd.h>
#include<time.h>
#include <signal.h>
#include <string.h>

#define BUF_SIZE 5

typedef struct {
    char *string;
    int number; 
    } 

data;




int main(int argc, char *argv[]) {


    int filedes[2];
    char buffer[BUF_SIZE];
    pid_t pid;
    data user_data;

    user_data.string = argv[1];
    user_data.number = atoi(argv[2]);

    

    if(pipe(filedes) == -1){
         perror("pipe failed");
         return -1;
    }

    
    pid = fork();

    if(pid > 0){

            close(filedes[0]);

            write(filedes[1], &user_data, sizeof(user_data));
          
            close(filedes[1]);

            wait(NULL);

    }

    if(pid == 0){

        close(filedes[1]); //close unused write

        for(;;){
            
            data child_data;

            int valid = read(filedes[0], &child_data, sizeof(user_data)); //reading data sent from parent

            if(valid == -1)
                exit(1);
        
            if(valid == 0)
                break;
        
            printf("name: %s\n", child_data.string);
            printf("age: %d\n", child_data.number);


        }



        close(filedes[0]);

        exit(1);

    }

        
    
            
}
       


