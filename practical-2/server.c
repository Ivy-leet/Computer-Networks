#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

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

    // Receive a message from client
    while ((read_size=recv(client_sock, client_message, 2000, 0)) >0)
        write(client_sock, client_message, strlen(client_message));

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
