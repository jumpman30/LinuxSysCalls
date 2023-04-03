#include<stdio.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<unistd.h>
#include<time.h>
#include <signal.h>
#include <string.h>

#define ARRAY_SIZE 1000
#define CHILD_NUM 5




int main() {

    int filedes[2];
    int vec1[ARRAY_SIZE], vec2[ARRAY_SIZE];
    int total_sum=0;
    pid_t child[5], pid;
    int status;
    time_t t;
    int fds[5][2];
    int start_reading = 0;
    int stop_reading = 200;


    srand ((unsigned) time (&t));

    for (int i = 0; i < ARRAY_SIZE; i++){
        vec1[i] = rand () % 1000;
        vec2[i] = rand () % 1000;
    }

        for(int i=0; i<CHILD_NUM; i++){
            pipe(fds[i]);
        }

    for(int i=0; i<CHILD_NUM && stop_reading<=ARRAY_SIZE; i++){

        if(i!=0){
            start_reading+=200;
            stop_reading+=200;
        }

        switch(child[i]=fork()){

            case -1:
                exit(-1);
        
            case 0:
                if(close(fds[i][0])==-1)
                    exit(-1);

                int sum=0;

                for(int i=start_reading; i<stop_reading;i++){
                    sum+=vec1[i] + vec2[i];
                }

                int rad = write(fds[i][1], &sum, sizeof(sum));
                printf("written %d bytes\n", rad);

                close(fds[i][1]);
            
                exit(1);

            
        
            default:
                break;

        }
    }

    for(int i=0; i<CHILD_NUM; i++){
        close(fds[i][1]);
    } 

       
    for(int i = 0; i < CHILD_NUM; i++){
        
        int dummy;

        pid = waitpid(child[i], &status, 0 );

        if(WIFEXITED(status)){
            read(fds[i][0], &dummy, sizeof(dummy));
            total_sum+=dummy;
            printf("Child %d sent %d\n", pid, dummy);
        }
    }

    printf("Total sum is %d\n", total_sum);

    close(filedes[0]);
      
}

