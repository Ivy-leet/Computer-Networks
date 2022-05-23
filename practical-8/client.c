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
#include <errno.h>
#include <sys/types.h>
#include <sys/inotify.h>
#include <limits.h>
#include <unistd.h>

#define PORT 21
#define STACK_MAX 100
#define MAX_EVENTS 1024 /*Max. number of events to process at one go*/
#define LEN_NAME 1024 /*Assuming length of the filename won't exceed 16 bytes*/
#define EVENT_SIZE  ( sizeof (struct inotify_event) ) /*size of one event*/
#define BUF_LEN     ( MAX_EVENTS * ( EVENT_SIZE + LEN_NAME )) /*buffer to store the data of events*/

void sendFile(int);
void get_event (int fd) {
    char buffer[BUF_LEN];
    int length, i = 0;

    length = read( fd, buffer, BUF_LEN );
    if ( length < 0 ) {
        perror( "read" );
    }

    while ( i < length ) {
        struct inotify_event *event = ( struct inotify_event * ) &buffer[ i ];
        if ( event->len ) {
            /*if ( event->mask & IN_CREATE) {
                if (event->mask & IN_ISDIR)
                    printf( "The directory %s was Created.\n", event->name );
                else
                    printf( "The file %s was Created with WD %d\n", event->name, event->wd );
            }*/

            if ( event->mask & IN_MODIFY) {
                if (event->mask & IN_ISDIR)
                    printf( "The directory %s was modified.\n", event->name );
                else
                    printf( "The file %s was modified with WD %d\n", event->name, event->wd );
            }

            /*if ( event->mask & IN_DELETE) {
                if (event->mask & IN_ISDIR)
                    printf( "The directory %s was deleted.\n", event->name );
                else
                    printf( "The file %s was deleted with WD %d\n", event->name, event->wd );
            }*/
            i += EVENT_SIZE + event->len;
        }
    }
}

int main(int argc, char const *argv[])
{
    int server;
    long valread;
    struct sockaddr_in address;
    int addrlen=sizeof(address);

    //Creating socket
    if ((server=socket(AF_INET, SOCK_STREAM, 0))==-1) {
        perror("In socket");
        exit(EXIT_FAILURE);
    }

    address.sin_family=AF_INET;
    address.sin_addr.s_addr=0;
    address.sin_port=htons(PORT);

    memset(address.sin_zero, '\0', sizeof(address.sin_zero));

    // Connect to remote server
    if (connect(server, (struct sockaddr *)&address, sizeof(address))<0) {
        perror("In connect");
        exit(EXIT_FAILURE);
    }
    else {
        printf("Connection established\n");
    }

    char response[256]=""; 

    if (send(server, "USER x\r\n", strlen("USER x\r\n"),0)<0) {
        perror("send failed");
        return 1;
    }

    if (send(server, "PASS x\r\n", strlen("PASS x\r\n"),0)<0) {
        perror("send failed");
        return 1;
    }

    if (send(server, "PASV\r\n", strlen("PASV\r\n"),0)<0) {
        perror("send failed");
        return 1;
    }

    int n;
    char ip[256];
    while ( (n = read(server, response, sizeof(response)-1)) > 0)
    {
        response[n] = 0;
        if(fputs(response, stdout) == EOF)
        {
            printf("\n Error : Fputs error\n");
            return 0;
        }

        if (strstr(response, "227")) {
            char delimit[]=" (),";

            char* string[256];

            int i=0;

            string[i]=strtok(response, delimit) ;

            char ip[256]="";
            char port[256]="";
            while (string[i]!=NULL)
            {
                if (i==4 || i==5 || i==6 || i==7) {
                    strcat(ip, string[i]);
                    strcat(ip, ".");
                }

                if (i==8) {
                    strcat(ip, "/");  
                    strcat(ip, string[i]);
                                 
                }

                if (i==9) {
                    strcpy(port, string[i]);
                }
                printf("string [%d]=%s\n", i, string[i]);
                i++;
                string[i]=strtok(NULL, delimit);
            }
            printf("%s\n", ip);
            printf("%s\n", port);
            printf("%s", response);

            int server_1;
            long valread;
            struct sockaddr_in address_1;
            int addrlen=sizeof(address_1);

            //Creating socket
            if ((server_1=socket(AF_INET, SOCK_STREAM, 0))==-1) {
                perror("In socket");
                exit(EXIT_FAILURE);
            }

            address_1.sin_family=AF_INET;
            address_1.sin_addr.s_addr=0;
            address_1.sin_port=htons(PORT);

            memset(address_1.sin_zero, '\0', sizeof(address_1.sin_zero));

            // Connect to remote server_1
            if (connect(server_1, (struct sockaddr *)&address_1, sizeof(address_1))<0) {
                perror("In connect");
                exit(EXIT_FAILURE);
            }
            else {
                printf("Connection established\n");
            }


            
            // strcpy(ip,response);
        }
    }

    printf("%s\n", ip);

    if(n < 0)
    {
        printf("\n Read error \n");
        return 0;
    } 

    /*
    int i = 0;
    char buffer[256]="";
    char response[256]=""; 

    strcpy(buffer, "USER tlholo\n");
    send(server, buffer, strlen(buffer), 0);
    recv(server, response, sizeof(response), 0);
    printf("%s\n", response);
    */
    // memset(buffer, 0, strlen(buffer));
    

    // if (strstr(response, "220")==NULL) {
    //     perror("Request not successful");
    //     exit(EXIT_FAILURE);
    // }
    // // memset(response, 0, strlen(response));
    /*
    char buffer1[256]="";
    char response1[256]="";
    strcpy(buffer1, "PASS Cativy08\n");
    printf("%s\n", buffer1);
    send(server, buffer1, strlen(buffer1), 0);
    recv(server, response1, sizeof(response1), 0);
    printf("%s\n", response1);
    memset(buffer, 0, strlen(buffer));
    memset(response, 0, strlen(response));
    */
    // char buffer2[256]="";
    // char response2[256]="";
    // strcpy(buffer2, "PASV\r\n");
    // printf("%s\n", buffer2);
    // send(server, buffer2, strlen(buffer2), 0);
    // recv(server, response2, sizeof(response2), 0);
    // printf("%s\n", response2);
    // // memset(buffer, 0, strlen(buffer));
    // printf("Here\n");
    // memset(response, 0, strlen(response));

    while (1) {
        // printf("We're within");

        /*
        bool equal = false;
        printf("Waiting for new connection...\n\n");

        // Waiting for upcoming client
        if ((new_socket=accept(server, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) {
            perror("In accept");
            exit(EXIT_FAILURE);
        }

        send(server_fd, buffer, 256, 0);
        recv(server_fd, &size, sizeof(int), 0);
        */
        close(server);
    }

    int wd, fd;

    fd = inotify_init();
    if ( fd < 0 ) {
        perror( "Couldn't initialize inotify");
    }

    wd = inotify_add_watch(fd, argv[1], IN_CREATE | IN_MODIFY | IN_DELETE);
    if (wd == -1) {
        printf("Couldn't add watch to %s\n",argv[1]);
    } else {
        printf("Watching:: %s\n",argv[1]);
    }

    /* do it forever*/
    while(1) {
        get_event(fd);
    }

    /* Clean up*/
    inotify_rm_watch( fd, wd );
    close( fd );

    
    return 0;
}


void sendFile(int socket) {
    // Malcolm, your code here

    if (send(socket, "STOR ./HTMLFolder/index.html\r\n", strlen("STOR ./HTMLFolder/index.html\r\n"),0)<0) {
        perror("send failed");
        return 1;
    }
}
