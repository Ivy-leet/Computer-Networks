#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define database ("database.txt")
#define MAX_LENGTH 257

/*struct Person{
    int id;
    char surnam[50];
    char name[50];
    int phoneNumber;
}*/

int search(char nam[MAX_LENGTH]){
    FILE *fptr;
    int id = -1;
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
            id = 0;
            break;
        }
        i++;
    }
    fclose(fptr);
    return id;
}

void viewAll(int client_sock){
    FILE *fptr;
    fptr = fopen(database, "r");

    if(fptr == NULL){
        printf("Error");
    }

    char buffer[MAX_LENGTH];
    char newline[2] ="\n";

    while(fgets(buffer, MAX_LENGTH, fptr)){
        buffer[strcspn(buffer, "#")] = 0;
        write(client_sock, buffer, strlen(buffer));
       // write(client_sock, newline, 2);
    }
}

void addition(char input[MAX_LENGTH]){
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

    char buff[] = "Welcome to your PhoneBook!!!\n";

    write(client_sock, buff, sizeof(buff));

    char buf[MAX_LENGTH] = "Enter a Corresponding number to Perform Action: (1)View all Contacts, (2)Search For Contact, (3)Update a Contact, (4)Delete Contact, (5)Insert a New Contact: ";

    write(client_sock, buf, MAX_LENGTH);
    // Receive a message from client
    while ((read_size=recv(client_sock, client_message, 2000, 0)) >0){
        //write(client_sock, client_message, strlen(client_message));
        printf("You wrote: %s", client_message);

        switch (client_message[0]) {
        //View ALl
        case '1':
            viewAll(client_sock);
            break;
        //Search
        case '2':
            while(true){
                char bufff[MAX_LENGTH] = "Enter Name of Contact you Want to Find: ";
                write(client_sock, bufff, MAX_LENGTH);

                if((read_size=recv(client_sock, client_message, 2000, 0)) > 0){
                    if(search(client_message)!= -1){

                        break;
                    }else{
                        char bu[MAX_LENGTH] = "Contact Not Found. \nSeacrh Again?: (0)YES, (ELSE)NO";
                        write(client_sock, bu, MAX_LENGTH);

                        if(read_size=recv(client_sock, client_message, 2000, 0) >0){
                            if(client_message[0] != 0){
                                break;
                            }
                        }
                    }
                }
            }
            break;
        //Update
        case '3':

            break;
        //Delete
        case '4':

            break;
        //Insert
        case '5':

            break;
        //Error
        default:
            printf("Error");
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
    
    close(socket_desc);

    return 0;
}
