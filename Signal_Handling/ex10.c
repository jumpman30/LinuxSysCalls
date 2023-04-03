//
// Created by Signals Pereira on 30/03/2023.
//
//
// Created by Signals Pereira on 28/03/2023.
//
//
// Created by Signals Pereira on 02/03/2023.
//

#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

volatile sig_atomic_t USR1_counter = 0;

void handle_SIGINT(int signo, siginfo_t *siginfo, void *context){
    char msg[80];

    USR1_counter++;
    sprintf ( msg , "Signal USR1 intercepted. Counter at %d \n", USR1_counter);
    write( STDOUT_FILENO  , msg , strlen ( msg ) ) ;
}

int main(void) {

    struct sigaction act;

    memset(&act, 0 , sizeof(struct sigaction));
    sigfillset(&act.sa_mask);
    sigdelset(&act.sa_mask, SIGUSR1);

    act.sa_sigaction = handle_SIGINT;
    act.sa_flags = SIGINFO;
    sigaction(SIGUSR1, &act, NULL);

    while(1) {
        printf("Process %d sleeping\n", getpid());
        sleep(1);
    }

}




