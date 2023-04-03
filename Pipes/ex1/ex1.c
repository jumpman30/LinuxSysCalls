#include<stdio.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<unistd.h>
#include<time.h>
#include <signal.h>
#include <string.h>



int main() {


    int filedes[2];

    if(pipe(filedes) == -1)
         exit(-1);


    switch(fork()){

        case -1:
            exit(-1);
        
        case 0: //child process
            if(close(filedes[1])==-1) //closing unused write end
                exit(-1);
            
            int parent_pid=0;

            for(;;){

                int valid =  read(filedes[0], &parent_pid, sizeof(parent_pid)); //reading data sent from parent

                if(valid == -1)
                    exit(-1);

            
                if(valid == 0)
                    break;
                
                else
                    printf("Parent sent: %d\n", parent_pid);

            }

            if(close(filedes[0])==-1)
                exit(-1);

            
            exit(1);
        
        default:
            if(close(filedes[0]) == -1)
                exit(-1);
            
            int pid = getpid();
            
            write(filedes[1], &pid, sizeof(pid_t));   

            if(close(filedes[1])==-1) //child will see end of file
                exit(-1);

            wait(NULL); //waiting for the child process to end 
            
            break;

    }
       


      
   
}

