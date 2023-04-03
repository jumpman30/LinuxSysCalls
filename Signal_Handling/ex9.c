

#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

void handle_SIGUSR1(int signo, siginfo_t *siginfo, void *context){
    char msg[80];
    sprintf ( msg , "Signal %d intercepted \n", siginfo->si_signo);
    write( STDOUT_FILENO  , msg , strlen ( msg ) ) ;
}

int main(void) {

    struct sigaction act;

    memset(&act, 0 , sizeof(struct sigaction));
    sigfillset(&act.sa_mask);
    sigdelset(&act.sa_mask, SIGINT);

    act.sa_sigaction = handle_SIGUSR1;
    act.sa_flags = SIGINFO;
    sigaction(SIGINT, &act, NULL);
    sigaction(SIGQUIT, &act, NULL);

    while(1) {
        printf("Process %d sleeping\n", getpid());
        sleep(1);
    }


}




