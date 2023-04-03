#include<stdio.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<unistd.h>
#include<time.h>
#include <signal.h>
#include <string.h>

#define ARRAY_SIZE 1000
#define CHILD_NUM 5
#define WRITE 1
#define READ 0




int main() {


    int filedes[2];
    pid_t child[5], pid;
    int status;
    time_t t;
    int fds[CHILD_NUM+1][2];
   
    

        for(int i=0; i<CHILD_NUM + 1; i++){
            if(pipe(fds[i])==-1){
                perror("Pipe failed");
                return -1;
            }
        }

    for(int i=0; i<CHILD_NUM; i++){
    
        int pid=fork();

        if(pid==-1){
            perror("Fork failed");
            return -1;
        }

        if(pid==0){
            int previous;
            close(fds[i][WRITE]);
            read(fds[i][READ], &previous, sizeof(int));

            srand((int)time(&t) % getpid());
            int number = rand () % (1+500) +1;

            printf("Process %d generated %d\n", getpid(), number);

            if(number>previous)
                write(fds[i+1][WRITE], &number, sizeof(int));
            
            else
                write(fds[i+1][WRITE], &previous, sizeof(int));

            close(fds[i][WRITE]);
            exit(1);
        }

    }

    
    close(fds[0][READ]);

    srand((int)time(&t) % getpid());
    int number = rand () % (1+500) +1;
    printf("Parent generated %d\n", number);
    write(fds[0][WRITE], &number, sizeof(int));
    close(fds[0][WRITE]);
    close(fds[CHILD_NUM][WRITE]);


    int greatest;
    read(fds[CHILD_NUM][READ], &greatest, sizeof(int));

    close(fds[CHILD_NUM][READ]);

    printf("Greatest number is %d\n", greatest);
    

    
      
}

