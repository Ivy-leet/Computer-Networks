#include <stdio.h>
#include <stdbool.h>
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
        //buffer[strcspn(buffer, "\n")] = 0;
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
    printf("%s\n",nam);
    FILE *fptr;

    fptr = fopen(database, "r");

    if(fptr == NULL){
        printf("Error");
        _exit(1);
    }

    char buffer[MAX_LENGTH];

    int i = 1;
    while(fgets(buffer, MAX_LENGTH, fptr)){
        // char* temp=strchr(buffer,'#');
        printf("%s", buffer);
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
        //buffer[strcspn(buffer, "\n")] = 0;
        write(client_sock, buffer, strlen(buffer));
        // write(client_sock, newline, 2);
    }

    fclose(fptr);
    write(client_sock, "\n", strlen("\n"));
}

void addition(char input[MAX_LENGTH]){
    FILE *fptr;
    fptr = fopen(database, "a");

    if(fptr == NULL){
        printf("Error");
    }

    input[strcspn(input, "\n")] = 0;
    fputs(input, fptr);
    // fputs("\n", fptr);

    fclose(fptr);
}

/*
int update(char nam[MAX_LENGTH]){
    
    FILE *fptr;
    int id = -1;
    fptr = fopen(database, "a");

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
}*/


bool delet(char input[MAX_LENGTH]){

    int id, cntl=0;
    id =search(input);

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

    while (!feof(fptr1)){
        strcpy(str, "\0");
        fgets(str, MAX_LENGTH, fptr1);

        if (!feof(fptr1)){
            cntl++;

            if (cntl!=id)
                fprintf(fptr2, "%s", str);
        }
    }

    fclose(fptr1);
    fclose(fptr2);
    remove(database);
    rename("data2.txt", database);

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

    while (true){
        char buf[MAX_LENGTH] = "Enter corresponding number to perform action\n(1)View all Contacts\n(2)Search For Contact\n(3)Update a Contact\n(4)Delete Contact\n(5)Insert a New Contact\n(clear)Clear screen\n(quit)Quit Phonebook\n> ";

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
                // while(1){
                {
                
                char bufff[MAX_LENGTH] = "\nEnter Name of Contact you Want to Find: ";
                write(client_sock, bufff, MAX_LENGTH);

            if((read_size=recv(client_sock, client_message, 2000, 0)) > 0)
                viewRecord(search(client_message), client_sock);
                    
            break;
            }
        //Update
        case '3':
	{
		char bufff[MAX_LENGTH] = "\nEnter Name of Contact you Want to Update: ";
                write(client_sock, bufff, MAX_LENGTH);

                if((read_size=recv(client_sock, client_message, 2000, 0)) > 0){
                int doesExist=search(client_message);
                viewRecord(doesExist, client_sock);

                if (delet(client_message)) {
                    char b[MAX_LENGTH] = "Enter New Contact Details(SameNumberAsAbove Surname {ContactName} PhoneNumber): ";
                    write(client_sock, b, MAX_LENGTH);

                    if((read_size=recv(client_sock, client_message, 2000, 0)) > 0){
                        addition(client_message);
                }   
                
            }
		   
                }
            break;
	}
        //Delete
        case '4':
            {
                char bufff[MAX_LENGTH] = "\nEnter Name of Contact you Want to delete: ";
                write(client_sock, bufff, MAX_LENGTH);
                
                if((read_size=recv(client_sock, client_message, 2000, 0)) > 0){
                    delet(client_message);
                    write(client_sock, "\n", 2);
                    viewAll(client_sock);
                    /*if (delete(client_message))
                        bufff[MAX_LENGTH]="Record successfully deleted!";
                    else
                        bufff[MAX_LENGTH]="Record could not be deleted!";
            */
                    
                }
                //write(client_sock, bufff, MAX_LENGTH);
                break;
        }
                
            //Insert
            case '5':
        {
            viewAll(client_sock);
            char b[MAX_LENGTH] = "Enter Contact Details(lastNumberAsSeenFromAbove+1 Surname {ContactName} PhoneNumber): ";
                write(client_sock, b, MAX_LENGTH);

            if((read_size=recv(client_sock, client_message, 2000, 0)) > 0){
                        addition(client_message);
                }

                break;
        }
            case 'c':
        {
                char b[MAX_LENGTH]="\e[2J \e[0;0H";
                write(client_sock, b, MAX_LENGTH);
                break;
        }
            case 'q':
        {
            close(client_sock);
            printf("Client has disconnected\n");
            break;
        }
            //Error
            default:
                printf("Error");

        }

        // buf[MAX_LENGTH] = "Enter corresponding number to perform action\n(1)View all Contacts\n(2)Search For Contact\n(3)Update a Contact\n(4)Delete Contact\n(5)Insert a New Contact\n(clear)Clear screen\n(quit)Quit Phonebook\n> ";

        write(client_sock, buf, MAX_LENGTH);
    
        }
	// char buf[MAX_LENGTH] = "\n\nEnter a Corresponding number to Perform Action: (1)View all Contacts, (2)Search For Contact, (3)Update a Contact, (4)Delete Contact, (5)Insert, (else)Quit: ";

	//     write(client_sock, buf, MAX_LENGTH);
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
