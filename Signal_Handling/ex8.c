

#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

void handle_USRR1(int signo, siginfo_t *siginfo, void *context){
    char msg[80];
    sprintf ( msg , " Process %d sent this signal\n" , siginfo->si_pid);
    write( STDOUT_FILENO  , msg , strlen ( msg ) ) ;
}

int main(void) {

    struct sigaction act;
    pid_t child;

    memset(&act, 0 , sizeof(struct sigaction));
    sigfillset(&act.sa_mask);
    sigdelset(&act.sa_mask, SIGUSR1);

    act.sa_sigaction = handle_USRR1;
    act.sa_flags = SIGINFO;
    sigaction(SIGUSR1, &act, NULL);


    if(( child = fork() ) == 0){
        //CHILD sends SIGUSR1 to parent process
        kill(getppid(), SIGUSR1);
        exit(0);
    }

    waitpid(child, NULL, 0);
    exit(1);

}




