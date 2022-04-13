/*
* 20575085, Koma T(Tlholo)
* 20456078, Dzimati BM(Malcolm)
*/

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

#define database ("database.txt")
#define MAX_LENGTH 256

void viewRecord(int num, int client_sock){
    

    if (num==-1){
        char buffer[MAX_LENGTH]="Record does not exist!\n";
        write(client_sock, buffer, strlen(buffer));
        return;
    }


    FILE *fptr;
    fptr = fopen(database, "r");

    if(fptr == NULL){
        printf("Error");
    }

    char buffer[MAX_LENGTH];
    char newline[2] ="\n";


    while(fgets(buffer, MAX_LENGTH, fptr)){
        if (buffer[0]==num+'0') {
            write(client_sock,"Record found:\t", strlen("Record found:\t"));
            write(client_sock, buffer, strlen(buffer));
            write(client_sock, newline, 2);
            return;
        }
        
    }

    fclose(fptr);
}

int search(char nam[MAX_LENGTH]){
    nam[strlen(nam)-1]=0;
    nam[strlen(nam)-2]=0;
    
    FILE *fptr;

    fptr = fopen(database, "r");

    if(fptr == NULL){
        printf("Error");
        _exit(1);
    }

    char buffer[MAX_LENGTH];

    int i = 1;
    while(fgets(buffer, MAX_LENGTH, fptr)){
        if (strstr(buffer, nam)!=NULL){
            fclose(fptr);
            return i;
        } 
        i++;
    }
    fclose(fptr);

    return -1;
}

void viewAll(int client_sock){
    FILE *fptr;
    fptr = fopen(database, "r");

    if(fptr == NULL){
        printf("Error");
    }

    char buffer[MAX_LENGTH];
    char newline[2] ="\n";

    write(client_sock, "\n", 2);
    while(fgets(buffer, MAX_LENGTH, fptr)){
        write(client_sock, buffer, strlen(buffer));
    }

    fclose(fptr);
    write(client_sock, "\n", strlen("\n"));
}

void addition(char input[MAX_LENGTH]){
    int id, cntl=0;

    char str[MAX_LENGTH];

    FILE *fptr1, *fptr2;

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

    int i = '1';

    while (!feof(fptr1)){
        strcpy(str, "\0");
        fgets(str, MAX_LENGTH, fptr1);

        if (!feof(fptr1)){
            str[0] = i;
            fprintf(fptr2, "%s", str);
            i++;
        }
    }
    input[0] = i;
    input[strcspn(input, "\n")] = 0;
    fprintf(fptr2, "%s", input);
    fprintf(fptr2, "%s", "\n");

    fclose(fptr1);
    fclose(fptr2);
    remove(database);
    rename("data2.txt", database);

}


bool delet(char input[MAX_LENGTH]){
    int id = search(input);
    if (id==-1) return false;

    char str[MAX_LENGTH];

    FILE *fptr1, *fptr2;

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
    char cntl = '1';
    char idCh = id + '0';

    while (!feof(fptr1)){
        fgets(str, MAX_LENGTH, fptr1);
            if (cntl!=idCh){
                str[0] = cntl;
               fprintf(fptr2, "%s", str);
		cntl++;
           }
    }

    fclose(fptr1);
    fclose(fptr2);
    remove(database);
    rename("data2.txt", database);
}


void *connection_handler(void *);


int main(int argc, char const *argv[])
{
    int socket_desc, client_sock, c, read_size, *new_sock;
    struct sockaddr_in server, client;
    char client_message[2000];

    // Create socket
    socket_desc=socket(AF_INET, SOCK_STREAM, 0);

    if (socket_desc==-1)
        printf("Couldn't create socket");
    
    puts("Socket created");

   server.sin_addr.s_addr=INADDR_ANY;
   server.sin_family=AF_INET;
   server.sin_port=htons(8888);

    // Connect to remote server
    if (bind(socket_desc, (struct sockaddr*)&server, sizeof(server)) <0) {
        puts("connect error");
        return 1;
    }

    puts("Connected\n");

    listen(socket_desc, 3);

    // Accept incoming connection
    puts("Waiting for incoming connection...");
    c=sizeof(struct sockaddr_in);

    // Accept connection from incoming client
    while( (client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c)) )
	{
		puts("Connection accepted");
		
		pthread_t sniffer_thread;
		new_sock = malloc(1);
		*new_sock = client_sock;
		
		if( pthread_create( &sniffer_thread , NULL ,  connection_handler , (void*) new_sock) < 0)
		{
			perror("could not create thread");
			return 1;
		}
		
		puts("Handler assigned");
	}
	
	if (client_sock < 0)
	{
		perror("accept failed");
		return 1;
	}
    // client_sock=accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);
    // if (client_sock<0) {
    //     perror("accept failed");
    //     return 1;
    // }
    // puts("Connection accepted");

    return 0;
}


void *connection_handler(void *socket_desc){
    int sock = *(int*)socket_desc;
	int read_size;
	char *message , client_message[2000];

    char buff[] = "Welcome to your PhoneBook!!!\n";

    write(sock, buff, sizeof(buff));

    while (true){
        char buf[MAX_LENGTH] = "Enter corresponding number to perform action\n(1)View all Contacts\n(2)Search For Contact\n(3)Update a Contact\n(4)Delete Contact\n(5)Insert a New Contact\n(clear)Clear screen\n(quit)Quit Phonebook\n> ";

        write(sock, buf, MAX_LENGTH);
        // Receive a message from client
        while ((read_size=recv(sock, client_message, 2000, 0)) >0){
            //write(sock, client_message, strlen(client_message));

            switch (client_message[0]) {
            //View ALl
            case '1':

                viewAll(sock);
                break;

            //Search
            case '2':{
                
                char bufff[MAX_LENGTH] = "\nEnter Name of Contact you Want to Find: ";
                write(sock, bufff, MAX_LENGTH);

                if((read_size=recv(sock, client_message, 2000, 0)) > 0)
                    viewRecord(search(client_message), sock);
                        
                break;
            }
        //Update
            case '3':{
                char bufff[MAX_LENGTH] = "\nEnter Name of Contact you Want to Update: ";
                write(sock, bufff, MAX_LENGTH);

                if((read_size=recv(sock, client_message, 2000, 0)) > 0){
                  int doesExist=search(client_message);
                  viewRecord(doesExist, sock);

                  if (doesExist!=-1) {
                    delet(client_message);
                    char b[MAX_LENGTH] = "Enter New Contact Details(SameNumberAsAbove Surname {ContactName} PhoneNumber): ";
                    write(sock, b, MAX_LENGTH);

                    if((read_size=recv(sock, client_message, 2000, 0)) > 0){
                        addition(client_message);
                    }
                
                  }
                  char c[MAX_LENGTH] = "Restart Phonebook to save changes!!\n";
                  write(sock, c, MAX_LENGTH);
                }
            break;
	}
        //Delete
        case '4':
            {
                char bufff[MAX_LENGTH] = "\nEnter Name of Contact you Want to delete: ";
                write(sock, bufff, MAX_LENGTH);
                
                if((read_size=recv(sock, client_message, 2000, 0)) > 0){
                    delet(client_message);
                    write(sock, "\n", 2);
                    viewAll(sock);
                }
                break;
            }
                
            //Insert
            case '5':
        {
            //viewAll(sock);
            char b[MAX_LENGTH] = "Enter Contact Details(  Surname {ContactName} PhoneNumber)(Begin with two empty spaces): ";
                write(sock, b, MAX_LENGTH);

                if((read_size=recv(sock, client_message, 2000, 0)) > 0)
                    addition(client_message);

            char c[MAX_LENGTH] = "Restart PhoneBook to save channges!!\n";
            write(sock, c, MAX_LENGTH);

                break;
            }
            case 'c':{
                char b[MAX_LENGTH]="\e[2J \e[0;0H";
                write(sock, b, MAX_LENGTH);
                break;
            }
            case 'q':{
                close(sock);
                printf("Client has disconnected\n");
                break;
            }
            //Error
            default:
                printf("Error");

        }

        write(sock, buf, MAX_LENGTH);
    
        }

    }

    if(read_size == 0)
	{
		puts("Client disconnected");
		fflush(stdout);
	}
	else if(read_size == -1)
	{
		perror("recv failed");
	}
    
    free(socket_desc);
}
