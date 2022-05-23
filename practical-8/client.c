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

    int i = 0;
    char buffer[256];
    char response[256]; 

    strcpy(buffer, "USER x\r\n");
    send(server, buffer, strlen(buffer), 0);
    recv(server, response, sizeof(response), 0);
    printf("%s\n", response);
    // memset(buffer, 0, strlen(buffer));
    

    if (strstr(response, "220")==NULL) {
        perror("Request not successful");
        exit(EXIT_FAILURE);
    }
    memset(response, 0, strlen(response));

    char buffer1[256];
    strcpy(buffer1, "PASS x\r\n");
    send(server, buffer1, strlen(buffer1), 0);
    recv(server, response, sizeof(response), 0);
    printf("%s\n", response);
    // memset(buffer, 0, strlen(buffer));
    memset(response, 0, strlen(response));

    char buffer2[256];
    strcpy(buffer2, "PASV\r\n");
    send(server, buffer2, strlen(buffer2), 0);
    recv(server, response, sizeof(response), 0);
    printf("%s\n", response);
    // memset(buffer, 0, strlen(buffer));
    printf("Here\n");
    memset(response, 0, strlen(response));

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
