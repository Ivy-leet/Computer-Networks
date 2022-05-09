/*
* 20575085, Koma T(Tlholo)
* 20456078, Dzimati BM(Malcolm)
*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <sys/stat.h>
#include <time.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 389
#define MAX 256

int main(int argc, char const *argv[])
{
    int sockfd, connfd;
    struct sockaddr_in servaddr, cli;

    

    if (sockfd=socket(AF_INET, SOCK_STREAM, 0)==-1) {
        printf("Cannot create socket...\n");
        exit(EXIT_FAILURE);
    }
    else
        printf("Socket successfully created...\n");
    
    address.sin_family=AF_INET;
    address.sin_addr.s_addr=inet_addr("127.0.0.1");
    address.sin_port=htons(PORT);

    if (connect(sockfd, (struct sockaddr_in*)&sockfd, sizeof(servaddr))!=0) {
        printf("Connection with the server failed...\n");
        exit(EXIT_FAILURE);
    }
    else 
        printf("Connected to the server...\n");

    while (1)
    {
        /* code */
    }

    close(sockfd);
    


    return 0;
}
