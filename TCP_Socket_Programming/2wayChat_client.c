/*
    C client example using sockets
*/
#include<stdio.h> //printf
#include<string.h>    //strlen
#include<sys/socket.h>    //socket
#include<arpa/inet.h> //inet_addr
#include<pthread.h>
#include<unistd.h>    //write
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
        
        if (response == -1) 
        {
          fprintf(stderr, "recv() failed\n");
          break;
        } 
        else if (response == 0) 
        {
              printf("\nPeer disconnected\n");
              break;
        } 
        else 
        {
              printf("\n>>%s", message);
              fflush(stdout); 
        }
    }
}


int main(int argc , char *argv[])
{
    int sock;
    struct sockaddr_in server;
    char message[MESSAGE_BUFFER];
    pthread_t receive;
    //Create socket
    sock = socket(AF_INET , SOCK_STREAM , 0);
    if (sock == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");
     
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons( 8888 );
 
    //Connect to remote server
    if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        perror("connect failed. Error");
        return 1;
    }
     
    puts("Connected\n");
     
    pthread_create(&receive, NULL, receive1, (void *) sock);
    while (fgets(message, MESSAGE_BUFFER, stdin) != NULL) 
    {
        if (strncmp(message, "/q", 2) == 0) 
        {
            printf("Closing connection...\n");
            exit(0);
        }
        write(sock , message , strlen(message));
    }
    close(sock);
    return 0;
}
