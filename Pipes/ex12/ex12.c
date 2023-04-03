#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#define CHILD_NUM 5
#define WRITE 1
#define READ 0

 typedef struct {
	    int preco;
	    char nome[50];
}info;

    typedef struct {
        int id;
        char nome[50];
}order;


int main(void)
{
    int fds[CHILD_NUM][2];
    int fd[2];



	info info_parent;
    order order_child;

    pid_t p;

    int nChild = 5;

    if(pipe(fd) == -1){
        perror("pipe fail");
        exit(-1);
	}

  
    order order_asked[5];
	order_asked[0].id = 0;
    strcpy(order_asked[0].nome, "shoes");

    order_asked[1].id = 1;
    strcpy(order_asked[1].nome, "ball");

    order_asked[2].id = 2;
    strcpy(order_asked[2].nome, "macbook");

    order_asked[3].id = 3;
    strcpy(order_asked[3].nome, "coca-cola");

    order_asked[4].id = 4;
    strcpy(order_asked[4].nome, "mouse");
    
    for(int i = 0; i < 5 ; i++){
		if(pipe(fds[i]) == -1){ 
			perror("pipe falhou");
		}	

		p = fork(); 

		if(p == -1){ 
			perror("fork falhou");
		}

		if(p == 0){
			close(fds[i][WRITE]);

            write(fd[WRITE], &order_asked[i], sizeof(order_asked)); 

			if(read(fds[i][READ], &info_parent, sizeof(info)) > 0){ 
				printf("\nBarcode nº%d\nName: %s\nPrice: %d€\n", i+1,info_parent.nome, info_parent.preco); 
			}

			close(fds[i][READ]); 
			exit(1); 
		}
    }  
    close(fd[WRITE]);  

    
    info product[5];
    product[0].preco = 10;
    strcpy(product[0].nome, "shoes");

    product[1].preco = 1;
    strcpy(product[1].nome, "ball");

    product[2].preco = 2;
    strcpy(product[2].nome, "macbook");

    product[3].preco = 3;
    strcpy(product[3].nome, "coca-cola");

    product[4].preco = 6;
    strcpy(product[4].nome, "mouse");
    
    for(int i = 0; i < nChild; i++){
        close(fds[i][READ]);

        if(read(fd[READ], &order_child, sizeof(order)) > 0){
             write(fds[order_child.id][WRITE], &product[i], sizeof(info));
        }
        close(fds[i][WRITE]); 
    }
    close(fd[WRITE]);
    return 0;
}
