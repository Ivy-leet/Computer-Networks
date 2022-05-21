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
#include <openssl/ssl.h>
#include <openssl/err.h>


#define PORT 25
#define SSLPORT 465

const char* getIPAddr(const char*);
int connectToServer(const char*);
int connectToSSLServer(const char*);
char* MailHeader(const char* from, const char* to, const char* subject, const char* mime_type, const char* charset);
void sendAlert(char* subject, char* body, char* strings[]);

SSL_CTX* InitCTX(void)
{
    SSL_METHOD *method;
    SSL_CTX *ctx;
    OpenSSL_add_all_algorithms();  
    SSL_load_error_strings();   
    method = TLSv1_2_client_method();  
    ctx = SSL_CTX_new(method);   
    if ( ctx == NULL )
    {
        ERR_print_errors_fp(stderr);
        abort();
    }
    return ctx;
}
void ShowCerts(SSL* ssl)
{
    X509 *cert;
    char *line;
    cert = SSL_get_peer_certificate(ssl); 
    if ( cert != NULL )
    {
        printf("Server certificates:\n");
        line = X509_NAME_oneline(X509_get_subject_name(cert), 0, 0);
        printf("Subject: %s\n", line);
        free(line);       
        line = X509_NAME_oneline(X509_get_issuer_name(cert), 0, 0);
        printf("Issuer: %s\n", line);
        free(line);       
        X509_free(cert);    
    }
    else
        printf("Info: No client certificates configured.\n");
}

bool promptUser() {
    printf ("Do you want to call the police or stay idle in the house?\nCall Police=0, Idle=1\n");
    printf(">");
    int integer;

    scanf("%d", &integer);

    return (integer==0);
}

int main(int count, char* strings[])
{
    // "System" to be integrated with email client.
    int randomNum;
    while (1) {
        randomNum=rand();

        if (randomNum%3==0)
        {
            printf("Door alarm has been tripped!\n");
            char subject[] = "Alert: Door Alarm Tripped";
            char body[] = "Dear Tlholo\n\nThis is an automated alert in response to movement by or inside the house.\n\nIf this was by unauthorised personal consider alerting the police.\n\nSincerely\nAlarm Buddy.";
            sendAlert(subject, body, strings);
            if (promptUser()) break;
        } 
        else if (randomNum%3==2) {
            printf("Gate alarm has been tripped!\n");
            char subject[] = "Alert: Gate Alarm Tripped";
            char body[] = "Dear Tlholo\n\nThis is an automated alert in response to movement by or inside the house.\n\nIf this was by unauthorised personal consider alerting the police.\n\nSincerely\nAlarm Buddy.";
            sendAlert(subject, body, strings);
            if (promptUser()) break;
        } 
        else if (randomNum%3==1) {
            printf("Window alarm has been tripped!\n");
            char subject[] = "Alert: Window Alarm Tripped";
            char bdy[] = "Dear Tlholo\n\nThis is an automated alert in response to movement by or inside the house.\n\nIf this was by unauthorised personal consider alerting the police.\n\nSincerely\nAlarm Buddy.";
            sendAlert(subject, bdy, strings);
            if (promptUser()) break;
        }
    }

    return 0;

}

void sendAlert(char* subject, char*  body, char* strings[]){
    char* From = "batsirai332@gmail.com";
    char* To = "tlholo332@gmail.com";                     //Body
    char *header = MailHeader(From, To, subject, "text/plain", "US-ASCII");

    SSL_CTX *ctx;
    int server = connectToServer("smtp.gmail.com");
    SSL *ssl;
    char buf[1024];
    char acClientRequest[1024] = {0};
    int bytes;
    char *hostname, *portnum;

    int recvd = 0;
    const char recv_buff[4768];
    int sdsd;
    sdsd = recv(server, recv_buff + recvd, sizeof (recv_buff) - recvd, 0);
    recvd += sdsd;

    printf("\nHandShake Over Secure Port 587\n\n");

    char buff[1000];
    strcpy(buff, "EHLO ");
    strcat(buff, "localhost");
    strcat(buff, "\r\n");
    send(server, buff, strlen(buff), 0);
    sdsd = recv(server, recv_buff + recvd, sizeof (recv_buff) - recvd, 0);
    recvd += sdsd;

    printf("%s\n", recv_buff);

    char _cmd3[1000];
    strcpy(_cmd3, "STARTTLS");
    strcat(_cmd3, "\r\n");
    send(server, _cmd3, strlen(_cmd3), 0);
    sdsd = recv(server, recv_buff + recvd, sizeof (recv_buff) - recvd, 0);
    recvd += sdsd;

    printf("%s\n", recv_buff);

    SSL_library_init();
    hostname=strings[1];
    portnum=strings[2];
    ctx = InitCTX();
    ssl = SSL_new(ctx);      
    SSL_set_fd(ssl, server);    
    if (SSL_connect(ssl)<=0) 
        ERR_print_errors_fp(stderr);
    else
    {
        printf("connected\n\n");
        printf("\nBeginning Of Communication Over Secure Socket Layer\n\n");

        char recv_buff[5000];
        char buffer[1000];
        strcpy(buffer, "EHLO ");
        strcat(buffer, "[127.0.0.1]");
        strcat(buffer, "\r\n");
        SSL_write(ssl, buffer, strlen(buffer));
        SSL_read(ssl, recv_buff, sizeof (recv_buff));
        printf("EHLO: %s\n",recv_buff);

        bzero(&recv_buff, sizeof(recv_buff));
        char _cmd2[1000];
        strcpy(_cmd2, "AUTH LOGIN\r\n");
        SSL_write(ssl, _cmd2, strlen(_cmd2));
        sdsd = SSL_read(ssl, recv_buff, sizeof (recv_buff));

        bzero(&recv_buff, sizeof(recv_buff));

        char _cmd1[1000];
        char* UID = "YmF0c2lyYWkzMzJAZ21haWwuY29t";
        strcpy(_cmd1, UID);
        strcat(_cmd1, "\r\n");
        SSL_write(ssl, _cmd1, strlen(_cmd1));
        sdsd = SSL_read(ssl, recv_buff, sizeof (recv_buff));

        printf("Reponses From Specific Requests:\n\n");

        char _cmd4[1000];
        char* PWD = "Q09TMzMyUHJhYw==";
        strcpy(_cmd4, PWD);
        strcat(_cmd4, "\r\n");
        SSL_write(ssl, _cmd4, strlen(_cmd4));
        sdsd = SSL_read(ssl, recv_buff, sizeof (recv_buff));
        printf("AUTH LOGIN: %s\n",recv_buff);

        bzero(&recv_buff, sizeof(recv_buff));

        char _cmd5[1000];
        strcpy(_cmd5, "MAIL FROM: ");
        strcat(_cmd5, "<batsirai332@gmail.com>");
        strcat(_cmd5, "\r\n");
        SSL_write(ssl, _cmd5, strlen(_cmd5));
        sdsd = SSL_read(ssl, recv_buff, sizeof (recv_buff));
        printf("MAIL FROM: %s\n",recv_buff);

        bzero(&recv_buff, sizeof(recv_buff));

        char _cmd6[1000];
        strcpy(_cmd6, "RCPT TO: ");
        strcat(_cmd6, "<tlholo332@gmail.com>");
        strcat(_cmd6, "\r\n");
        SSL_write(ssl, _cmd6, strlen(_cmd6));
        sdsd = SSL_read(ssl, recv_buff, sizeof (recv_buff));
        printf("RCPT TO: %s\n", recv_buff);

        bzero(&recv_buff, sizeof(recv_buff));

        char _cmd7[1000];
        strcpy(_cmd7, "DATA\r\n");
        SSL_write(ssl, _cmd7, strlen(_cmd7));
        sdsd = SSL_read(ssl, recv_buff, sizeof (recv_buff));
        printf("DATA: %s\n", recv_buff);

        bzero(&recv_buff, sizeof(recv_buff));

        SSL_write(ssl, header, strlen(header));
        char _cmd8[1000];
        strcat(_cmd8, body);
        strcat(_cmd8, "\r\n.\r\n");
        SSL_write(ssl, _cmd8, strlen(_cmd8));
        sdsd = SSL_read(ssl, recv_buff, sizeof (recv_buff));
        printf("BODY: %s\n",recv_buff);

        bzero(&recv_buff, sizeof(recv_buff));

        char _cmd10[1000];
        strcpy(_cmd10, "QUIT\r\n");
        SSL_write(ssl, _cmd10, strlen(_cmd10));
        sdsd = SSL_read(ssl, recv_buff, sizeof (recv_buff));
        printf("QUIT: %s\n",recv_buff);

    }
    
    free(header);
    close(server);
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
    int socket_fd;

    socket_fd=socket(AF_INET, SOCK_STREAM, IPPROTO_IP);

    struct sockaddr_in address;
    bzero(&address, sizeof(address));
    address.sin_family=AF_INET;
    
    address.sin_port=htons(PORT);
    
    if (inet_pton(AF_INET, getIPAddr(server_add), &address.sin_addr)==1) {
        connect(socket_fd, (struct sockaddr*)&address, sizeof(address));
    }

    return socket_fd;
}

int connectToSSLServer(const char* server_add) {
    int socket_fd;

    socket_fd=socket(AF_INET, SOCK_STREAM, IPPROTO_IP);

    struct sockaddr_in address;
    bzero(&address, sizeof(address));
    address.sin_family=AF_INET;

    address.sin_port=htons(SSLPORT);

    if (inet_pton(AF_INET, getIPAddr(server_add), &address.sin_addr)==1) {
        connect(socket_fd, (struct sockaddr*)&address, sizeof(address));
    }
    return socket_fd;
}

char* MailHeader(const char* from, const char* to, const char* subject, const char* mime_type, const char* charset){
    time_t now;
    time(&now);
    char* mail_header = NULL;
    char date_buff[26];
    char Sender[6 + strlen(from) + 1 + 1];
    char Recip[4 + strlen(to) + 1 + 1];
    char Subject[8 + 1 + strlen(subject) + 1 + 1];
    char mime_data[13 + 1 + 3 + 1 + 1 + 13 + 1 + strlen(mime_type) + 1 + 1 + 8 + strlen(charset) + 1 + 1 + 2];

    strftime(date_buff, (33), "%a , %d %b %Y %H:%M:%S", localtime(&now));

    sprintf(Sender, "From: %s\r\n", from);
    sprintf(Recip, "To: %s\r\n", to);
    sprintf(Subject, "Subject: %s\r\n", subject);
    sprintf(mime_data, "MIME-Version: 1.0\r\nContent-type: %s; charset=%s\r\n\r\n", mime_type, charset);

    int mail_header_length = strlen(Subject) + strlen(Sender) + strlen(Recip) + strlen(mime_type) + 59;

    mail_header = (char*) malloc(mail_header_length * sizeof (char));

    memcpy(&mail_header[0], &Sender, strlen(Sender));
    memcpy(&mail_header[0 + strlen(Sender)], &Recip, strlen(Recip));
    memcpy(&mail_header[0 + strlen(Sender) + strlen(Recip)], &Subject, strlen(Subject));
    memcpy(&mail_header[0 + strlen(Sender) + strlen(Recip) + strlen(Subject)], &mime_data, strlen(mime_data));
    return mail_header;
}

