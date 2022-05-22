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
#include <sys/sendfile.h>

#define PORT 21
#define STACK_MAX 100


int main(int argc, char const *argv[])
{
    
    int server_fd, new_socket;
    long valread;
    struct sockaddr_in address;
    int addrlen=sizeof(address);

    //Creating socket
    if ((server_fd=socket(AF_INET, SOCK_STREAM, 0))==-1) {
        perror("In socket");
        exit(EXIT_FAILURE);
    }

    address.sin_family=AF_INET;
    address.sin_addr.s_addr=0;
    address.sin_port=htons(PORT);

    memset(address.sin_zero, '\0', sizeof(address.sin_zero));

    // Connect to remote server
    if (connect(server_fd, (struct sockaddr *)&address, sizeof(address))<0) {
        perror("In connect");
        exit(EXIT_FAILURE);
    }

    int i = 0;
    char buffer[256];

    while (1) {
        printf("We're within");

        /*
        bool equal = false;
        printf("Waiting for new connection...\n\n");

        // Waiting for upcoming client
        if ((new_socket=accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) {
            perror("In accept");
            exit(EXIT_FAILURE);
        }

        send(server_fd, buffer, 256, 0);
        recv(server_fd, &size, sizeof(int), 0);
        */
        close(new_socket);
    }
    
    return 0;
}