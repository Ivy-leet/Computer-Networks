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
#include <arpa/inet.h>
#include <netdb.h>


#define PORT 25
#define MAX_LENGTH 256
#define database ("database.txt")

const char* getIPAddr(const char*);
int connectToServer(const char*);

int main(int argc, char const *argv[])
{
    int connectfd=connectToServer("smtp.gmail.com");

    if (connectfd!=0) {
        printf("connected to server!\n");
    }
    
    return 0;
}

const char* getIPAddr(const char* target_domain) {
    const char* target_ip;
    struct in_addr *host_address;
    struct hostent *raw_list=gethostbyname(target_domain);
    if (raw_list==NULL) {
        printf("Error: no such host name");
        exit(EXIT_FAILURE);
    }

    for (int i=0; raw_list->h_addr_list[i]!=0; i++) {
        host_address=raw_list->h_addr_list[i];
        target_ip=inet_ntoa(*host_address);
    }

    return target_ip;
}


int connectToServer(const char* server_add) {
    int socket_fd=socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
    
    struct sockaddr_in address;
    bzero(&address, sizeof(address));
    address.sin_family=AF_INET;
    
    address.sin_port=htons(PORT);
    
    if (inet_pton(AF_INET, getIPAddr(server_add), &address.sin_addr)==1) {
        connect(socket_fd, (struct sockaddr*)&address, sizeof(address));
    }
    return socket_fd;
}

char* MailHeader(const char* from, const char* to, const char* subject, const char* mime_type, const char* charset){
    time_t now;
    time(&now);
    char* mail_header = null;
    char date_buff[26];
    char Sender[6 + strlen(from) + 1 + 1];
    char Recip[4 + strlen(to) + 1 + 1];
    char Subject[8 + 1 + strlen(subject) + 1 + 1];
    char mime_data[13 + 1 + 3 + 1 + 1 + 13 + 1 + strlen(mime_type) + 1 + 1 + 8 + strlen(charset) + 1 + 1 + 2];

    strftime(date_buff, (33), "%a , %d %b %Y %H:%M:%S", localtime($now));

    sprintf(Sender, "From: %s\r\n", from);
    sprintf(Recip, "To: %s\r\n", to);
    sprintf(Subject, "Subject: %s\r\n", subject);
    sprintf(mime_date, "MIME-Version: 1.0\r\nContent-type: %s; charset=%s\r\n\r\n", mime_type, charset);

    int mail_header_length = strlen(Subject) + strlen(Sender) + strlen(Recip) + strlen(mime_type) + 10;

    mail_header = (char*) malloc(mail_heard_length * sizeof (char));

    memcpy(&mail_header[0], &Sender, strlen(Sender));
    memcpy(&mail_header[0 + strlen(Sender)], &Recip, strlen(Recip));
    memcpy(&mail_header[0 + strlen(Sender) + strlen(Recip)], &Subject, strlen(Subject));
    memcpy(&mail_header[0 + strlen(Sender) + strlen(Recip) + strlen(Subject)], &mime_data, strlen(mime_data));
    return mail_header;
}


