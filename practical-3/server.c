#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 5555

void setHttpHeader(char*);
void response(struct sockaddr_in* );
void getCalculator(char[], int, char[]);
char* getHeader(int number);

int main(int argc, char const *argv[])
{
    int server_fd, new_socket;
    long valread;
    struct sockaddr_in address;
    int addrlen=sizeof(address);

    char* hello="HTTP/1.1 200 OK\nContent-Type: text/html;charser=UTF-8\nContent-Length: 500\n\n<?xml>\n<!DOCTYPE hmtl>\n<html>\n<head>\n<h1>Test</h1></head>\n<body>hee</body>\n</html>";

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
        char tes[] = "";
        getCalculator(tes, new_socket, NULL);
        //write(new_socket, hello, strlen(hello));

        printf("----------Hello message sent-------------\n");
        close(new_socket);
    }
    return 0;
}


void setHttpHeader(char httpRequestHeader[]) {
    char httpResponseHeader[2000]="HTTP/1.1 200 OK\nContent-Type: text/plain\n\n";

    char buttonPressed=httpRequestHeader[4];
}

/*char* toArr(int number){
    int n = log10(number)+1;
    int i;
    char* numArr= callol(n, sizeof(char));
    for(i = n-1; i >= 0; --i, number /= 10){
        numArr[i] = (number % 10)+'0';
    }
    return numArr;
}*/


void getCalculator(char header[], int s, char ans[]){
    header = "HTTP/1.1 200 OK\nContent-Type: text/html;charser=UTF-8\nContent-Length: 1000\n\n<?xml>\n<!DOCTYPE hmtl>\n<html>\n<head></head>\n<body><table style='width:100%'><tr><th colspan='4'><h1>0</h1></th></tr><tr><th><a href=''>7</a></th><th><a href=''>8</a></th><th><a href=''>9</a></th><th><a href=''>/</a></th></tr><tr><th><a href=''>4</a></th><th><a href=''>5</a></th><th><a href=''>6</a></th><th><a href=''>X</a></th></tr><tr><th><a href=''>1</a></th><th><a href=''>2</a></th><th><a href=''>3</a></th><th><a href=''>-</a></th></tr><tr><th><a href=''>0</a></th><th><a href=''>.</a></th><th><a href=''>+</a></th><th><a href=''>=</a></th></tr></table></body>\n</html>";
    write(s, header, strlen(header));
}

/*
char* 9etHeader(int len){
/  hello =
    char strl[11];
    sprintf(strl, "%ld", len);
    strncat(ret, strl, strlen(strl))X
    printf(ret);
    retu1n ret;
}
*/
