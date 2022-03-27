#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define database ("database.txt")
#define MAX_LENGTH 256

/*struct Person{
    int id;
    char surnam[50];
    char name[50];
    int phoneNumber;
}*/

void search(int id){
    FILE *fptr;
    fptr = fopen(database, "r");

    if(fptr == NULL){
        printf("Error");
        _exit(1);
    }

    char buffer[MAX_LENGTH];

    int i = 0;
    while(fgets(buffer, MAX_LENGTH, fptr)){
        if(id == i){
            printf("%s", buffer);
            break;
        }
        i++;
    }
    fclose(fptr);
}

void viewAll(){
    FILE *fptr;
    fptr = fopen(database, "r");

    if(fptr == NULL){
        printf("Error");
    }

    char buffer[MAX_LENGTH];

    int i = 0;
    while(fgets(buffer, MAX_LENGTH, fptr)){
        printf("%s", buffer);
    }
}

void addition(char input[256]){
    FILE *fptr;
    fptr = fopen(database, "w");

    if(fptr == NULL){
        printf("Error");
    }

    char buffer[MAX_LENGTH];

    while(fgets(buffer, MAX_LENGTH, fptr)){

    }

    fputs(input, fptr);


    fclose(fptr);
}



int main(int argc, char const *argv[])
{
    int socket_desc, client_sock, c, read_size;
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
    client_sock=accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);
    if (client_sock<0) {
        perror("accept failed");
        return 1;
    }
    puts("Connection accepted");

    char buff[] = "Welcome to our database!!!\n";

    write(client_sock, buff, sizeof(buff));

    // Receive a message from client
    while ((read_size=recv(client_sock, client_message, 2000, 0)) >0){
        write(client_sock, client_message, strlen(client_message));
        printf("You wrote: %s", client_message);
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
    
    close(socket_desc);

    return 0;
}
