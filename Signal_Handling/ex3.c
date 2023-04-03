
#include <unistd.h>
#include <stdio.h>
#include <signal.h>

int main() {
    int process_id, signal_id;

    printf("Enter process id: \n");
    scanf("%d", &process_id);

    printf("Enter signal id: \n");
    scanf("%d", &signal_id);

    int result = kill(process_id, signal_id);
    if(result != 0){
        perror("Kill failed");
        return -1;
    }
    sleep(2);
    printf("Restarting process...\n");
    result = kill(process_id, SIGCONT);
    if(result != 0){
        perror("Kill failed");
        return -1;
    }

    return 0;
}

