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


#define PORT 587
#define MAX_LENGTH 256
#define database ("database.txt")

const char* getIPAddr(const char*);
int connectToServer(const char*);
char* MailHeader(const char* from, const char* to, const char* subject, const char* mime_type, const char* charset);


SSL_CTX* InitCTX(void)
{
    SSL_METHOD *method;
    SSL_CTX *ctx;
    OpenSSL_add_all_algorithms();  /* Load cryptos, et.al. */
    SSL_load_error_strings();   /* Bring in and register error messages */
    method = TLSv1_2_client_method();  /* Create new client-method instance */
    ctx = SSL_CTX_new(method);   /* Create new context */
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
    cert = SSL_get_peer_certificate(ssl); /* get the server's certificate */
    if ( cert != NULL )
    {
        printf("Server certificates:\n");
        line = X509_NAME_oneline(X509_get_subject_name(cert), 0, 0);
        printf("Subject: %s\n", line);
        free(line);       /* free the malloc'ed string */
        line = X509_NAME_oneline(X509_get_issuer_name(cert), 0, 0);
        printf("Issuer: %s\n", line);
        free(line);       /* free the malloc'ed string */
        X509_free(cert);     /* free the malloc'ed certificate copy */
    }
    else
        printf("Info: No client certificates configured.\n");
}

int main(int count, char* strings[])
{
    char* From = "batsirai332@gmail.com";
    char* To = "tlholo332@gmail.com";
    char *header = MailHeader("batsirai332@gmail.com", "tlholo332@gmail.com", "Hello Its a test Mail From Momo", "text/plain", "US-ASCII");
    
     printf("I am here\n");

    SSL_CTX *ctx;
    int server;
    SSL *ssl;
    char buf[1024];
    char acClientRequest[1024] = {0};
    int bytes;
    char *hostname, *portnum;
    // if ( count != 3 )
    // {
    //     printf("usage: %s <hostname> <portnum>\n", strings[0]);
    //     exit(0);
    // }
    SSL_library_init();
    hostname=strings[1];
    portnum=strings[2];
    ctx = InitCTX();
    server = connectToServer("ssl://smtp.gmail.com");
    ssl = SSL_new(ctx);      /* create new SSL connection state */
    SSL_set_fd(ssl, server);    /* attach the socket descriptor */
    if ( SSL_connect(ssl) == -1 )   /* perform the connection */
        ERR_print_errors_fp(stderr);
    else
    {
        printf("connected\n");
        int recvd = 0;
        const char recv_buff[4768];
        int sdsd;
        sdsd = recv(server, recv_buff + recvd, sizeof (recv_buff) - recvd, 0);
        recvd += sdsd;

        char buff[1000];
        strcpy(buff, "EHLO ");
        strcat(buff, "localhost");
        strcat(buff, "\r\n");
        send(server, buff, strlen(buff), 0);
        sdsd = recv(server, recv_buff + recvd, sizeof (recv_buff) - recvd, 0);
        recvd += sdsd;

        //printf("%s\n", recv_buff);


        char _cmd2[1000];
        strcpy(_cmd2, "AUTH CRAM-md5\r\n");
        int dfdf = send(server, _cmd2, strlen(_cmd2), 0);
        sdsd = recv(server, recv_buff + recvd, sizeof (recv_buff) - recvd, 0);
        recvd += sdsd;

        printf("%s\n", recv_buff);

        char _cmd3[1000];
        char* UID = "YmF0c2lyYWkzMzJAZ21haWwuY29t";
        strcpy(_cmd3, "batsirai332@gmail.com");
        strcat(_cmd3, "\r\n");
        send(server, _cmd3, strlen(_cmd3), 0);
        sdsd = recv(server, recv_buff + recvd, sizeof (recv_buff) - recvd, 0);
        recvd += sdsd;

        //printf("%s\n", recv_buff);

        char _cmd4[1000];
        char* PWD = "Q09TMzMyUHJhYw==";
        strcpy(_cmd4, "COS332Prac");
        strcat(_cmd4, "\r\n");
        send(server, _cmd4, strlen(_cmd4), 0);
        sdsd = recv(server, recv_buff + recvd, sizeof (recv_buff) - recvd, 0);
        recvd += sdsd;

        char _cmd5[1000];
        strcpy(_cmd5, "MAIL FROM: ");
        strcat(_cmd5, "<batsirai332@gmail.com>");
        strcat(_cmd5, "\r\n");
        send(server, _cmd5, strlen(_cmd5), 0);
        sdsd = recv(server, recv_buff + recvd, sizeof (recv_buff) - recvd, 0);
        recvd += sdsd;

        char _cmd6[1000];
        strcpy(_cmd6, "RCPT TO: ");
        strcat(_cmd6, "<tlholo332@gmail.com>");
        strcat(_cmd6, "\r\n");
        send(server, _cmd6, strlen(_cmd6), 0);
        sdsd = recv(server, recv_buff + recvd, sizeof (recv_buff) - recvd, 0);
        recvd += sdsd;

        char _cmd7[1000];
        strcpy(_cmd7, "DATA\r\n");
        send(server, _cmd7, strlen(_cmd7), 0);
        sdsd = recv(server, recv_buff + recvd, sizeof (recv_buff) - recvd, 0);
        recvd += sdsd;

        send(server, header, strlen(header), 0);
        char _cmd8[1000];
        strcpy(_cmd8, "Hello this is a test");
        send(server, _cmd8, sizeof (_cmd8), 0);
        char _cmd9[1000];
        strcpy(_cmd9, "\r\n.\r\n");
        send(server, _cmd9, sizeof (_cmd9), 0);
        sdsd = recv(server, recv_buff + recvd, sizeof (recv_buff) - recvd, 0);
        recvd += sdsd;

        char _cmd10[1000];
        strcpy(_cmd10, "QUIT\r\n");
        send(server, _cmd10, sizeof (_cmd10), 0);
        sdsd = recv(server, recv_buff + recvd, sizeof (recv_buff) - recvd, 0);
        recvd += sdsd;
        printf("%s\n", recv_buff);
    }
    
    
    // int connectfd=connectToServer("smtp.gmail.com");
    // if (connectfd!=0) {
    //     printf("connected to server!\n");
        
        
    // }
    
    free(header);
    close(server);

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

    printf("I am here\n");
    int mail_header_length = strlen(Subject) + strlen(Sender) + strlen(Recip) + strlen(mime_type) + 100;

    mail_header = (char*) malloc(mail_header_length * sizeof (char));
     printf("I am here\n");

    memcpy(&mail_header[0], &Sender, strlen(Sender));
     printf("I am here\n");
    memcpy(&mail_header[0 + strlen(Sender)], &Recip, strlen(Recip));
    memcpy(&mail_header[0 + strlen(Sender) + strlen(Recip)], &Subject, strlen(Subject));
    memcpy(&mail_header[0 + strlen(Sender) + strlen(Recip) + strlen(Subject)], &mime_data, strlen(mime_data));
     printf("I am here\n");
    return mail_header;
}


