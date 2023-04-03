
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int simulate1();
void simulate2();
#define CHILD_COUNT 50

volatile sig_atomic_t USR1_counter = 0;

void handle_SIGUSR1(int signo, siginfo_t *siginfo, void *context){
    USR1_counter++;
    char msg[80];
    sprintf ( msg , "Signal USR1 intercepted by child %d.\n", getpid());
}

void handle_SIGUSR1_by_child(int signo, siginfo_t *siginfo, void *context){
    simulate2();
}

int main(void) {

    struct sigaction act;
    srand(time(NULL));
    pid_t pid[CHILD_COUNT];

    memset(&act, 0 , sizeof(struct sigaction));
    sigfillset(&act.sa_mask);
    sigdelset(&act.sa_mask, SIGUSR1);

    act.sa_sigaction = handle_SIGUSR1;
    act.sa_flags = SIGINFO;
    sigaction(SIGUSR1, &act, NULL);

    for(int i = 0; i < CHILD_COUNT; i++) {

        if ((pid[i] = fork()) == 0){
            act.sa_sigaction = handle_SIGUSR1_by_child;
            act.sa_flags = SIGINFO;
            sigaction(SIGUSR1, &act, NULL);
            if(simulate1()){
                kill(getppid(), SIGUSR1);
            }
            else
                kill(getppid(), SIGUSR1);

            sigwait(&act.sa_mask, NULL);
            exit(1);
        }
    }

    for(int i = 0; i < CHILD_COUNT / 2; i++){
        waitpid(pid[i], NULL, 0);
    }

    for(int i = 0; i < CHILD_COUNT; i++){
        kill(pid[i], SIGUSR1);
    }

}

int simulate1(){
   return rand() % 2;
}
void simulate2(){
    printf("Simulate 2 is executing by process %d", getpid());
}




