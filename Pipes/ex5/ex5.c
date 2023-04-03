#include<stdio.h>
#include<unistd.h>
#include<string.h>

#define BUFF_SIZE 256

void toUpperCase(char *message);

int main() {

   int pipefds1[2], pipefds2[2];
   int returnstatus1, returnstatus2;
   int pid;
   char readmessage[BUFF_SIZE];


   pipe(pipefds1);
   
   pipe(pipefds2);
   
   pid = fork();
   
   if (pid != 0) {

      char pipe1writemessage[BUFF_SIZE];
      fgets(pipe1writemessage, BUFF_SIZE, stdin);

      close(pipefds1[0]); 
      close(pipefds2[1]); 

      write(pipefds1[1], pipe1writemessage, sizeof(pipe1writemessage));

      read(pipefds2[0], readmessage, sizeof(readmessage));
      printf("In Parent: Reading from pipe 2 – Message is %s\n", readmessage);

   } else { 
      
      
      char readmessage[BUFF_SIZE];
      close(pipefds1[1]); // Close the unwanted pipe1 write side
      close(pipefds2[0]); // Close the unwanted pipe2 read side

      read(pipefds1[0], readmessage, sizeof(readmessage));
      printf("In Child: Reading from pipe 1 – Message is %s\n", readmessage);

      toUpperCase(readmessage);

   
      write(pipefds2[1], readmessage, sizeof(readmessage));

   }
   return 0;
}


void toUpperCase(char *str){

   for (int i = 0; i <= strlen (str); i++){  

      if (str[i] >= 'a' && str[i] <= 'z')  
      str[i] = str[i] - 32; 

 }  

}


