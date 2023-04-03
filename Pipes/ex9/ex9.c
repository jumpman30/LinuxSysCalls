#include<stdio.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<unistd.h>
#include<time.h>
#include <signal.h>
#include <string.h>

#define ARRAY_SIZE 1000

  typedef struct{
     int customer_code;
     int product_code;
     int quantity;
}
sales;



int main() {


    int filedes[2];
    sales array_sales[ARRAY_SIZE];
    int total_sum=0;
    int *products = (int *)malloc(sizeof(int));
    pid_t child[5], pid;
    int status;
    time_t t;
    int start_reading = 0;
    int stop_reading = 200;
    char *files;

    strcpy(&files[0], "file1.txt");
    strcpy(&files[1], "file2.txt");
    strcpy(&files[2], "file3.txt");
    strcpy(&files[3], "file4.txt");
    strcpy(&files[4], "file5.txt");

    if(products == NULL)
        exit(-1);


    srand ((unsigned) time (&t));

    //generating numbers between 0 and 999
    for (int i = 0; i < ARRAY_SIZE; i++){
        array_sales[i].customer_code = rand () % 100; //generating 100 customers
        array_sales[i].product_code = rand () % 100; //generating 100 products
        array_sales[i].quantity = rand () % 100; //generating sales quantities between 0 and 100
    }


    if(pipe(filedes) == -1)
         exit(-1);

    for(int i=0; i<5 && stop_reading<=1000; i++){

        if(i!=0){
            start_reading+=200;
            stop_reading+=200;
        }

        switch(child[i]=fork()){

            case -1:
                exit(-1);
        
            case 0: //child process
                if(close(filedes[0])==-1) //closing unused read end
                    exit(-1);

                

                for(int i=start_reading; i<stop_reading;i++){
                    if(array_sales[i].quantity>20){
                         int rad = write(filedes[1], &array_sales[i].product_code, sizeof( array_sales[i].product_code));
                         //printf("written %d bytes\n", rad);

                    }
                }

                close(filedes[1]);
            
                exit(1);

            
        
            default:
                break;

        }
    }

    close(filedes[1]); //close unused write

       
    for(int i = 0; i < 5; i++){
        
        pid = waitpid(child[i], &status, 0 );
        if(WIFEXITED(status))
           printf("Child %d exited %d\n", pid, WEXITSTATUS(status));
    }

    int product_code;
    int total_products=1;

      for(;;){

            int valid = read(filedes[0], &product_code, sizeof(int)); //reading data sent from child

            if(valid == -1)
                exit(-1);
        
            if(valid == 0)
                break;
        
            //printf("product code: %d\n", dummy);
            products[total_products-1] = product_code;
            products = (int *)realloc(products, sizeof(int)*(total_products+1));
            total_products++; 
      }


    close(filedes[0]);

    int i=0;
    while(products[i]){
        printf("PRODUCT CODE: %d\n", products[i]);
        i++;
    }
    printf("total products that excedeed expectation was: %d\n", total_products-1);
      
}

