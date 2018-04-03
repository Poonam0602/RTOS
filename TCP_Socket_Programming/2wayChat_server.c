/*
    C socket server example
*/
 
#include<stdio.h>
#include<string.h>    //strlen
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include<unistd.h>    //write
#include<pthread.h>
#include<stdlib.h>

#define MESSAGE_BUFFER 5000

void * receive1(void * socket) 
{
    int socket_fd, response;
    char message[MESSAGE_BUFFER];
    socket_fd = (int) socket;

    // Print received message
    while(1) 
    {
        memset(message, 0, MESSAGE_BUFFER); // Clear message buffer
        response = recvfrom(socket_fd, message, MESSAGE_BUFFER, 0, NULL, NULL);
        if (response) 
        {
            printf("\n>>%s", message);
        }
    }
}


int main(int argc , char *argv[])
{
    int socket_desc , client_sock , c , read_size;
    struct sockaddr_in server , client;
    char client_message[MESSAGE_BUFFER];
    char reply[MESSAGE_BUFFER];
    pthread_t receive;
     
    //Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");
     
    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( 8888 );
     
    //Bind
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        //print the error message
        perror("bind failed. Error");
        return 1;
    }
    puts("bind done");
     
    //Listen
    listen(socket_desc , 3);
     
    //Accept and incoming connection
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);
     
    //accept connection from an incoming client
    client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);
    if (client_sock < 0)
    {
        perror("accept failed");
        return 1;
    }
    puts("Connection accepted");
	pthread_create(&receive,NULL,receive1,(void *) client_sock);
    while (fgets(reply, MESSAGE_BUFFER, stdin) != NULL) 
    {
        if (strncmp(reply, "/q", 2) == 0) 
        {
            printf("Closing connection...\n");
            exit(0);
        }
        write(client_sock , reply , strlen(reply));
    }

    close(socket_desc);
    close(socket_desc);
    pthread_exit(NULL);
    return 0;
}
