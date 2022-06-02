/*
* 20575085, Koma T(Tlholo)
* 20456078, Dzimati BM(Malcolm)
*/

#include <stdio.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <termios.h>
#include <fcntl.h>

#define PORT 5555
#define TELNETPORT 23
#define MAX_LENGTH 256

#define DO 0xfd
#define WONT 0xfc
#define WILL 0xfb
#define DONT 0xfe
#define CMD 0xff
#define CMD_ECHO 1
#define CMD_WINDOW_SIZE 31

void negotiate(int sock, unsigned char *buf, int len) {
    int i;
     
    if (buf[1] == DO && buf[2] == CMD_WINDOW_SIZE) {
        unsigned char tmp1[10] = {255, 251, 31};
        if (send(sock, tmp1, 3 , 0) < 0)
            exit(1);
         
        unsigned char tmp2[10] = {255, 250, 31, 0, 80, 0, 24, 255, 240};
        if (send(sock, tmp2, 9, 0) < 0)
            exit(1);
        return;
    }
     
    for (i = 0; i < len; i++) {
        if (buf[i] == DO)
            buf[i] = WONT;
        else if (buf[i] == WILL)
            buf[i] = DO;
    }
 
    if (send(sock, buf, len , 0) < 0)
        exit(1);
}

static struct termios tin;
 
static void terminal_set(void) {
    tcgetattr(STDIN_FILENO, &tin);

    static struct termios tlocal;

    memcpy(&tlocal, &tin, sizeof(tin));

    cfmakeraw(&tlocal);
    tcsetattr(STDIN_FILENO,TCSANOW,&tlocal);
}
 
static void terminal_reset(void) {
    tcsetattr(STDIN_FILENO,TCSANOW,&tin);
}

int main(int argc, char const *argv[])
{
    
    int server_fd, new_socket;
    long valread;
    struct sockaddr_in address;
    int addrlen=sizeof(address);
    int len;

    
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


    char buf[1024];

    struct timeval ts;
    ts.tv_sec = 1; // 1 second
    ts.tv_usec = 0;

    while (1) {
        bool equal = false;
        printf("Waiting for new connection...\n\n");

        if ((new_socket=accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) {
            perror("In accept");
            exit(EXIT_FAILURE);
        }

        printf("Connected\n");


        int sockfd, connfd;
        struct sockaddr_in servaddr, cli;

        sockfd=socket(AF_INET, SOCK_STREAM, 0);

        if (sockfd == -1) {
            perror("In socket creation");
            exit(EXIT_FAILURE);
        }

        bzero(&servaddr, sizeof(servaddr));

        servaddr.sin_family = AF_INET;
        servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
        servaddr.sin_port = htons(TELNETPORT);

        if (connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) != 0) {
            perror("In connect");
            exit(EXIT_FAILURE);
        }
        
        char buffer[1000]={0};
        valread=read(new_socket, buffer, 1000);

        if (strcmp(buffer, "telnet")==0) {
            fd_set fds;
            FD_ZERO(&fds);
            if (sockfd != 0)
                FD_SET(sockfd, &fds);
            FD_SET(0, &fds);

            int nready = select(sockfd + 1, &fds, (fd_set *) 0, (fd_set *) 0, &ts);
            if (nready < 0) {
                perror("select. Error");
                return 1;
            }
            else if (nready == 0) {
                ts.tv_sec = 1; // 1 second
                ts.tv_usec = 0;
            }
            else if (sockfd != 0 && FD_ISSET(sockfd, &fds)) {
                // start by reading a single byte
                int rv;
                if ((rv = recv(sockfd , buf , 1 , 0)) < 0)
                    return 1;
                else if (rv == 0) {
                    printf("Connection closed by the remote end\n\r");
                    return 0;
                }
    
                if (buf[0] == CMD) {
                    // read 2 more bytes
                    len = recv(sockfd , buf + 1 , 2 , 0);
                    if (len  < 0)
                        return 1;
                    else if (len == 0) {
                        printf("Connection closed by the remote end\n\r");
                        return 0;
                    }
                    negotiate(sockfd, buf, 3);
                }
                else {
                    len = 1;
                    buf[len] = '\0';
                    printf("%s", buf);
                    fflush(0);
                }
            }
            
            else if (FD_ISSET(0, &fds)) {
                buf[0] = getc(stdin);
                if (send(sockfd, buf, 1, 0) < 0)
                    return 1;
                if (buf[0] == '\n')
                    putchar('\r');
            }
        }
            printf("%s\n", buffer);

        close(server_fd);
    }
    
    
    return 0;
}