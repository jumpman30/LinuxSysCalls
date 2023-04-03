#include<stdio.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<unistd.h>
#include<time.h>
#include <signal.h>
#include <string.h>

#define ROUNDS 10
#define CHILD_NUM 5

typedef struct {
    char *str;
    int round; 
    } 

data;



int main(int argc, char *argv[]) {


    int filedes[2];
    pid_t child[CHILD_NUM], pid;
    int status;
    time_t t;
    data game_data;
    int winners[CHILD_NUM][2];

    game_data.str = argv[1];
    game_data.round=1;


    if(pipe(filedes) == -1)
         exit(-1);

    for(int i=0; i<CHILD_NUM; i++){

        switch(child[i]=fork()){

            case -1:
                exit(-1);
        
            case 0:
                if(close(filedes[1])==-1)
                    exit(-1);

                data child_data;


                read(filedes[0], &child_data, sizeof(game_data));
                printf("Winner Winner chicken dinner! || round: %d \n", child_data.round);

                close(filedes[0]);
            
                exit(child_data.round); 
        
            default:
                break;

        }
    }

    close(filedes[0]);

    for(int i=0;i<CHILD_NUM;i++){
        write(filedes[1], &game_data, sizeof(game_data));
        game_data.round++;
        sleep(2);
    }

       
    for(int i = 0; i < CHILD_NUM; i++){
        
        int dummy;

        pid = waitpid(child[i], &status, 0 );

        if(WIFEXITED(status)){
            int round = WEXITSTATUS(status);
            
            if(i>0 && winners[i-1][1] > round){
                int pid_aux = winners[i-1][0];
                int round_aux = winners[i-1][1];

                winners[i-1][0] = pid;
                winners[i-1][1] = round;

                winners[i][0] = pid_aux;
                winners[i][1] = round_aux;  
            }

            else{
                winners[i][0] = pid;
                winners[i][1] = round;
            }

        }
    }

    close(filedes[1]);

    for(int i=0; i<CHILD_NUM; i++){
        printf("child %d winner at round %d\n", winners[i][0], winners[i][1]);
    }

      
}

