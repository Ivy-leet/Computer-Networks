#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 5555

void setHttpHeader(char*);
void response(struct sockaddr_in* );

int main(int argc, char const *argv[])
{
    int server_fd, new_socket;
    long valread;
    struct sockaddr_in address;
    int addrlen=sizeof(address);

    char* hello="HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";

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

    while (1) {
        printf("Waiting for new connection...\n\n");

        // Waiting for upcoming client
        if ((new_socket=accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) {
            perror("In accept");
            exit(EXIT_FAILURE);
        }


        char buffer[1000]={0};
        valread=read(new_socket, buffer, 1000);
        printf("%s\n", buffer);
        write(new_socket, hello, strlen(hello));

        printf("----------Hello message sent-------------\n");
        close(new_socket);
    }
    return 0;
}


void setHttpHeader(char httpRequestHeader[]) {
    char httpResponseHeader[2000]="HTTP/1.1 200 OK\nContent-Type: text/plain\n\n";

    char buttonPressed=httpRequestHeader[4];
}