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

    while (0) {
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
    
    return 0;
}
