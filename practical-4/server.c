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

char* getCurrentDate();
char* getLastModifiedDate();
char* getContentLength();
void getFormInsert(char*);
void getFormSearch(char*);
void preResults(char*);
void postResults(char*);
void defa(char*);

void databaseFunctionality();
void viewAll();
int search(char nam[MAX_LENGTH]);
void insert(char buffer[MAX_LENGTH]);
bool delete(char input[MAX_LENGTH]);
int getLastIndex();

int main(int argc, char const *argv[])
{
    /*
    // For debugging purposes
    int num;
    while (true) {
        printf("\n\nMenu\n[1]View all\n[2]Search\n[3]Insert\n[4]Delete\n>");
        scanf("%d",&num);

        if (num==1)
            viewAll();
        else if (num==2) {
            printf ("Enter surname you want to search: ");
            char input[MAX_LENGTH];
            scanf("%s", input);
            search(input);
        }
        else if (num==3) {
            insert("His", "Her", "02478963142");
        }
        else if (num==4) {
            printf("Enter surname you want to delete: ");

            char input[MAX_LENGTH];
            scanf("%s", input);
            delete(input);
        }
    }
    */
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
    char site[3000];      //Global site variable

    while (1) {
        bool equal = false;
        printf("Waiting for new connection...\n\n");

        // Waiting for upcoming client
        if ((new_socket=accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) {
            perror("In accept");
            exit(EXIT_FAILURE);
        }


        char buffer[1000]={0};
        valread=read(new_socket, buffer, 1000);
        printf("%s\n", buffer);
        getFormInsert(site);
        // insert(buffer);
        write(new_socket, site, strlen(site));     //This displays the site 
        close(new_socket);
    }
    
    
    return 0;
}

char* getCurrentDate() {
    char static dateField[100]="";
    time_t rawtime=time(NULL);

    struct tm *ptm=localtime(&rawtime);
    strftime(dateField, 100, "Date: %a, %d %b %Y %H:%M:%S GMT\n", ptm);

    return dateField;
}

char* getLastModifiedDate() {
    char static lastModifiedField[100]="";
    struct stat filestat;

    stat("server.c", &filestat);

    struct tm *ftm=localtime(&filestat.st_mtime);

    strftime(lastModifiedField, 100, "Last-Modiefied: %a, %d %b %Y %H:%M:%S GMT\n", ftm);

    return lastModifiedField;
}

char* getContentLength(char* x) {
    char static buffer[100]="";
    int contentLength=strlen(x);
    snprintf(buffer, 100, "Content-Length: %d\n", contentLength);

    return buffer;

}

int getLastIndex() {
    FILE *fptr=fopen(database,"r");

    if (!fptr) {
        printf("Database not found!\n");
        _exit(1);
    }
    char buffer[MAX_LENGTH];

    int i=0;
    while (fgets(buffer, MAX_LENGTH, fptr)) {
        i++;
        // printf("Here\n");
    }

    // printf("%d\n", i);

    fclose(fptr);

    return i;
}

void viewAll() {
    FILE *fptr;
    fptr = fopen(database, "r");

    if(fptr == NULL){
        printf("Error");
    }

    char buffer[MAX_LENGTH];
    char newline[2] ="\n";

    while (fgets(buffer, MAX_LENGTH, fptr)) {
        printf("%s", buffer);
    }

    fclose(fptr);
}

int search(char nam[MAX_LENGTH]) {
    printf("%s\n", nam);
    FILE *fptr;

    fptr = fopen(database, "r");

    if(fptr == NULL){
        printf("Error");
        _exit(1);
    }

    char buffer[MAX_LENGTH];

    int i = -1;
    while(fgets(buffer, MAX_LENGTH, fptr)){
        if (strstr(buffer, nam)!=NULL){
            printf("%s", buffer);
            i++;
        } 
    }
    fclose(fptr);

    return i;
}

void insert(char buffer[MAX_LENGTH]) {

    char surname[MAX_LENGTH], name[MAX_LENGTH], num[MAX_LENGTH];

    char newBuffer[MAX_LENGTH];
    char newBuffer2[MAX_LENGTH];

    char* startChar;
    char* endChar;

    int startIndex;
    int endIndex;

    startChar=strchr(buffer, '?');
    endChar=strchr(buffer,'\n');

    startIndex=(int)(startChar-buffer);
    endIndex=(int)(endChar-buffer);
    
    strncpy(newBuffer, buffer+startIndex+1, endIndex-startIndex);

    //  endChar=strchr(newBuffer,' ');

    // // // startIndex=(int)(startChar-buffer);
    // endIndex=(int)(endChar-newBuffer);
    
    // strncpy(newBuffer2, newBuffer, endIndex-1);

    // printf("%s\n", newBuffer2);
    
    char delimit[]="&= ";
    char* string[MAX_LENGTH];

    int i=0, j=0;

    string[i]=strtok(newBuffer, delimit);
    while (string[i]!=NULL)
    {
        if (i==1)
            strcpy(surname, string[i]);
            // surname=string[i];
        else if (i==3)
            strcpy(name, string[i]);
            // name=string[i];
        else if (i==5)
            strcpy(num, string[i]);
            // num=string[i];

        printf("string [%d]=%s\n", i, string[i]);
        i++;
        string[i]=strtok(NULL, delimit);
    }

    printf("Surname: %s\nName: %s\nNumber: %s\n", surname, name, num);
    
    int index=getLastIndex()+1;
    char recordToInsert[MAX_LENGTH];

    sprintf(recordToInsert,"%d", index);

    strcat(recordToInsert, " ");
    strcat(recordToInsert, surname);
    strcat(recordToInsert, " ");
    strcat(recordToInsert, name);
    strcat(recordToInsert, " ");
    strcat(recordToInsert, num);

    // printf ("%s", recordToInsert);

    FILE *fptr=fopen(database,"a");

    if (!fptr) {
        printf("Database not found!\n");
        _exit(1);
    }
    
    fprintf(fptr, "\n");
    fputs(recordToInsert, fptr);

    fclose(fptr);
    
}

bool delete(char input[MAX_LENGTH]) {
    if (search(input)==-1) return false;

    FILE *fptr1, *fptr2;
    char str[MAX_LENGTH];

    fptr1=fopen(database, "r");

    if (!fptr1) {
        printf("Database not found!\n");
        _exit(1);
    }
    fptr2=fopen("data2.txt", "w");

    if (!fptr2) {
        printf("Unable to open temporary file\n");
        fclose(fptr1);
        _exit(0);
    }

    char cntl='1';

    while (!feof(fptr1))
    {
        fgets(str, MAX_LENGTH, fptr1);
        if (!strstr(str, input)) {
            str[0]=cntl;
            fprintf(fptr2, "%s", str);
            cntl++;
        }
    }

    fclose(fptr1);
    fclose(fptr2);
    remove(database);
    rename("data2.txt", database);
}

void getFormInsert(char* site){
    char* head = "HTTP/1.1 200 OK\nConnection: keep-alive\nContent-Type: text/html;charser=UTF-8\nCache-Control: max-age=604800\n";
    char* error = "HTTP/1.1 400 Bad Request\nConnection: keep-alive\nContent-Type: text/html;charser=UTF-8\nCache-Control: max-age=604800\n";

    char* date=getCurrentDate();
    char* lastModified=getLastModifiedDate();
    char* server="Server: Maverick\n";
    char* title="\n\n<?xml>\n<!DOCTYPE hmtl>\n<html>\n<head><style>\n</style>\n</head>\n<body>\n<h2>Insert Contact</h2>\n<form method='get' ><label for='fsname'>Surname Name:</label><br>\n<input type='text' id='fsname' name='fsname'><br>\n<label for='fname'>Name:</label><br>\n<input type='text' id='fname' name='fname'><br>\n<label for='fnumber'>Phone number:</label><br>\n<input type='text' id='fnumber' name='fnumber'><br>\n<input type='submit' value='Submit'>\n</form></body>\n</html>";

    char* contentLength=getContentLength(title);

    memset(site, 0, strlen(site));

    strcat(site, head);
    strcat(site, server);
    strcat(site, date);
    strcat(site, contentLength);
    strcat(site, lastModified);
    strcat(site, title);
    //strcat(header, ans);
    //strcat(header, body);
}

void getFormSearch(char* site){
    char* head = "HTTP/1.1 200 OK\nConnection: keep-alive\nContent-Type: text/html;charser=UTF-8\nCache-Control: max-age=604800\n";
    char* error = "HTTP/1.1 400 Bad Request\nConnection: keep-alive\nContent-Type: text/html;charser=UTF-8\nCache-Control: max-age=604800\n";

    char* date=getCurrentDate();
    char* lastModified=getLastModifiedDate();
    char* server="Server: Maverick\n";
    char* title="\n\n<?xml>\n<!DOCTYPE hmtl>\n<html>\n<head><style>\n</style>\n</head>\n<body>\n<h2>Search Contact</h2>\n<form method='get'><label for='fsearch'>name:</label><br>\n<input type='text' id='fsearch' name='fsearch'><br>\nnput type='submit' value='Submit'>\n</form></body>\n</html>";

    char* contentLength=getContentLength(title);

    memset(site, 0, strlen(site));

    strcat(site, head);
    strcat(site, server);
    strcat(site, date);
    strcat(site, contentLength);
    strcat(site, lastModified);
    strcat(site, title);
    //strcat(header, ans);
    //strcat(header, body);
}

