#include<stdio.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<unistd.h>
#include<time.h>
#include <signal.h>
#include <string.h>

#define ARRAY_SIZE 1000




int main() {


    int filedes[2];
    int vec1[ARRAY_SIZE], vec2[ARRAY_SIZE];
    int total_sum=0;
    pid_t child[5], pid;
    int status;
    time_t t;
    int start_reading = 0;
    int stop_reading = 200;


    srand ((unsigned) time (&t));

    //generating numbers between 0 and 999
    for (int i = 0; i < ARRAY_SIZE; i++){
        vec1[i] = rand () % 1000;
        vec2[i] = rand () % 1000;
    }


    if(pipe(filedes) == -1)
         exit(-1);

    for(int i=0; i<5 && stop_reading<=1000; i++){

        if(i!=0){
            start_reading+=200;
            stop_reading+=200;
        }

        switch(child[i]=fork()){

            case -1:
                exit(-1);
        
            case 0:
                if(close(filedes[0])==-1)
                    exit(-1);

                int sum=0;

                for(int j=start_reading; j<stop_reading;j++){
                    sum+=vec1[j] + vec2[j];
                }

                int rad = write(filedes[1], &sum, sizeof(sum));

                close(filedes[1]);
            
                exit(1);

            
        
            default:
                break;

        }
    }

    close(filedes[1]);

       
    for(int i = 0; i < 5; i++){
        
        int dummy;

        pid = waitpid(child[i], &status, 0 );

        if(WIFEXITED(status)){
            read(filedes[0], &dummy, sizeof(dummy));
            total_sum+=dummy;
            printf("Child %d sent %d\n", pid, dummy);
        }
    }

    printf("Total sum is %d\n", total_sum);

    close(filedes[0]);
      
}

