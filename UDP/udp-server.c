#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(){
    int port = 7077;
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);;
    struct sockaddr_in server_address, remote_address;
    char buffer[1024];
    socklen_t addr_size;

    memset(&server_address, '\0', sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");

    bind(sockfd, (struct sockaddr*)&server_address, sizeof(server_address));
    addr_size = sizeof(server_address);

    recvfrom(sockfd, buffer, 1024, 0, (struct sockaddr*)& remote_address, &addr_size);

    while(1) {
        printf("Got this: %s", buffer);
    }
    return 0;

}