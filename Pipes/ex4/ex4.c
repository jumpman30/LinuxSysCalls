#include<stdio.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<unistd.h>
#include<time.h>
#include <signal.h>
#include <string.h>

#define BUFF_SIZE 1

int main()
{
	FILE* ptr;
	char ch;
    int filedes[2];
    pid_t pid;
    int status;

    if(pipe(filedes) == -1){
         perror("pipe failed");
         return -1;
    }

    pid = fork();

    if(pid > 0){

        close(filedes[0]);
	
	    ptr = fopen("text.txt", "r");

	    if (NULL == ptr) {
		    printf("file can't be opened \n");
	    }

        do {
		    ch = fgetc(ptr);
            write(filedes[1], &ch, BUFF_SIZE);

		
	    } while (ch != EOF);

        close(filedes[1]);
	    fclose(ptr);

        pid_t child = waitpid(pid, &status, 0 );    
            if(WIFEXITED(status))
                printf("Child %d exited with status %d\n", child, WEXITSTATUS(status));
	    
    }

    if(pid == 0){
        
        close(filedes[1]);
        char data;

        for(;;){

            int valid = read(filedes[0], &data, BUFF_SIZE);

            if(valid == -1)
                exit(-1);
        
            if(valid == 0)
                break;
        
            printf("data: %c\n", data); 

        }



        close(filedes[0]);

        exit(45);

    }

}

