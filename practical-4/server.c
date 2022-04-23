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

void databaseFunctionality();
void viewAll();
int search(char nam[MAX_LENGTH]);
void insert(char surname[MAX_LENGTH], char name[MAX_LENGTH], char num[MAX_LENGTH]);
bool delete(char input[MAX_LENGTH]);
int getLastIndex();

int main(int argc, char const *argv[])
{
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
    }
    /*
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
    char header[3000];

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

        
        close(new_socket);
    }
    */
    
    return 0;
}

int getLastIndex() {
    FILE *fptr=fopen(database,"r");

    if (!fptr) {
        printf("Database not found!\n");
        _exit(1);
    }
    char buffer[MAX_LENGTH];

    int i=1;
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

void insert(char surname[MAX_LENGTH], char name[MAX_LENGTH], char num[MAX_LENGTH]) {
    int i=getLastIndex();
    char recordToInsert[MAX_LENGTH];

    sprintf(recordToInsert,"%d", i);

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
    
}

