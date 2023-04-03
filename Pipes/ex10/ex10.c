#include<stdio.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<unistd.h>
#include<time.h>
#include <signal.h>
#include <string.h>



int main() {


    int filedes[2];
    int credit = 20;
    int parent_child_pipe[2], child_parent_pipe[2];
    time_t t;
    pid_t pid;
    int sucess = 1;
    int fail = 0;


    pipe(parent_child_pipe);
   
    pipe(child_parent_pipe);


    pid=fork();

    if(pid>0){
       
        close(parent_child_pipe[0]);
        close(child_parent_pipe[1]);

        while(credit>0){
            
            srand((int)time(&t) % getpid());

  
          int number = rand () % 6;
          printf("Parent generated %d \n", number);
          write(parent_child_pipe[1], &sucess, sizeof(int)); 

          int bet;
          read(child_parent_pipe[0], &bet, sizeof(int));
          printf("child sent bet %d\n", bet);
          if(bet==number){
              credit = credit + 10;
          }
          else{
              credit = credit -5;
          }
          write(parent_child_pipe[1], &credit, sizeof(int)); 

        }

        write(parent_child_pipe[1], &fail, sizeof(int));
     
        close(parent_child_pipe[1]);
        close(child_parent_pipe[0]);

    }

    if(pid==0){
            srand((int)time(&t) % getpid());

        close(parent_child_pipe[1]);
        close(child_parent_pipe[0]);

        int data;
        read(parent_child_pipe[0], &data, sizeof(int));

        while(data!=0){
             int child_bet = rand () % 6;
             write(child_parent_pipe[1], &child_bet, sizeof(int));
             read(parent_child_pipe[0], &data, sizeof(int));
             printf("I have %d $$\n", data);
            read(parent_child_pipe[0], &data, sizeof(int));
        }



        close(parent_child_pipe[0]);
        close(child_parent_pipe[1]);

      
    }

      
   
}

