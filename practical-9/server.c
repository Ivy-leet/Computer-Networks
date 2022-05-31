/*
* 20575085, Koma T(Tlholo)
* 20456078, Dzimati BM(Malcolm)
*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <time.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 5555
#define MAX_LENGTH 256
#define database ("database.txt")

// returns name of text file for base64 encoding
char* getEncodingFileName(char*);

int main(int argc, char const *argv[])
{
    
    int server_fd, new_socket;
    long valread;
    struct sockaddr_in address;
    int addrlen=sizeof(address);

    //Creating socket
    if ((server_fd=socket(AF_INET, SOCK_STREAM, 0))==0) {
        perror("In socket");
        exit(EXIT_FAILURE);
    }

    address.sin_family=AF_INET;
    address.sin_addr.s_addr=INADDR_ANY;
    address.sin_port=htons(PORT);

    memset(address.sin_zero, '\0', sizeof(address.sin_zero));

    // Connect to remote server
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0) {
        perror("In bind");
        exit(EXIT_FAILURE);
    }

    // Adding listener
    if (listen(server_fd, 10)<0) {
        perror("In listen");
        exit(EXIT_FAILURE);
    }


    int i = 0;
    char site[3000000];      //Global site variable
    char results[3000];   // Results for search and view all

    while (1) {
        bool equal = false;
        printf("Waiting for new connection...\n\n");

        // Waiting for upcoming client
        if ((new_socket=accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) {
            perror("In accept");
            exit(EXIT_FAILURE);
        }

        printf("Connected\n");

        
        char buffer[1000]={0};
        valread=read(new_socket, buffer, 1000);

        // if (buffer=="telnet")
            printf("%s\n", buffer);

        
        // char m = buffer[5];
        // char j=buffer[6];
        // */
        

        
        close(new_socket);
    }
    
    
    return 0;
}