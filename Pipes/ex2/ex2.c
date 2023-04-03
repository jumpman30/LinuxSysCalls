#include<stdio.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<unistd.h>
#include<time.h>
#include <signal.h>
#include <string.h>

#define BUF_SIZE 5

int main(int argc, char *argv[]) {


    int filedes[2];
    char buffer[BUF_SIZE];
    pid_t pid;
    char *str = argv[1];
    char *number = argv[2];

    

    if(pipe(filedes) == -1){
         perror("pipe failed");
         return -1;
    }

    
    pid = fork();

    if(pid > 0){

            close(filedes[0]);

            write(filedes[1], str, strlen(str));
            write(filedes[1], number, strlen(str));
            
            close(filedes[1]);

            wait(NULL);

    }

    if(pid == 0){

        close(filedes[1]);

        for(;;){

            int valid = read(filedes[0], &buffer, BUF_SIZE);

            if(valid == -1)
                exit(1);
        
            if(valid == 0)
                break;
        
            printf("data: %s\n", buffer);

        }



        close(filedes[0]);

        exit(1);

    }

        
    
            
}
       


