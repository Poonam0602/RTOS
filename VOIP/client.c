/***
CLIENT
***/
/*command to compile:
cc -o client client.c -lpulse -lpulse-simple -lpthread
*/
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include<pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <pulse/simple.h>
#include <pulse/error.h>
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include<string.h>
#include<stdlib.h>
#define BUFSIZE 1024
#define MESSAGE_BUFFER 1024
#define SOCK_ADD 8887
#define SERVER "127.0.0.1"

int sock;
struct sockaddr_in client;
int length = sizeof(client);
int dropCall =0;
void * endCall1()
{
    char message[MESSAGE_BUFFER];
    while (1)
    {
        fgets(message, MESSAGE_BUFFER, stdin) ;
    
        if (strncmp(message, "/q", 2) == 0) 
        {
            printf("Closing connection...\n");
            dropCall = 1;
            break;
        }
    }
    return NULL;

}
static ssize_t loop_write(int fd, const void*data, size_t size) {
    ssize_t ret = 0;
    while (size > 0) {
        ssize_t r;
        if ((r = sendto(sock, data, size , 0 , (struct sockaddr *) &client, length))<=0)
            return r;
        if (r == 0)
            break;
        ret += r;
        data = (const uint8_t*) data + r;
        size -= (size_t) r;
    }
    return ret;
}
void * receive1() 
{
    pa_simple *s = NULL;
    int ret = 1;
    int error;
        static const pa_sample_spec cs = {
        .format = PA_SAMPLE_S16LE,
        .rate = 7000,
        .channels = 2
    };
    /* Create a new playback stream */
    if (!(s = pa_simple_new(NULL, "App", PA_STREAM_PLAYBACK, NULL, "playback", &cs, NULL, NULL, &error))) {
        fprintf(stderr, __FILE__": pa_simple_new() failed: %s\n", pa_strerror(error));
        if (s)
            pa_simple_free(s);
    }
    while(1)
    {
        uint8_t buf[BUFSIZE];
        ssize_t r;
        /* Read some data ... */
        if ((r = recvfrom(sock, buf, BUFSIZE, 0, (struct sockaddr *) &client, &length)) <= 0)  {
            if (r == 0) /* EOF */
                break;
            fprintf(stderr, __FILE__": read() failed: %s\n", strerror(errno));
            if (s)
                pa_simple_free(s);
        }
        /* ... and play it */
        if (pa_simple_write(s, buf, (size_t) r, &error) < 0) {
            fprintf(stderr, __FILE__": pa_simple_write() failed: %s\n", pa_strerror(error));
            if (s)
                pa_simple_free(s);
        }
        if(dropCall == 1)
            break;
    }
    /* Make sure that every single sample was played */
    if (pa_simple_drain(s, &error) < 0) {
        fprintf(stderr, __FILE__": pa_simple_drain() failed: %s\n", pa_strerror(error));
        if (s)
            pa_simple_free(s);
    }
    return NULL;

}
int main(int argc, char*argv[]) 
{
    int error;
    pthread_t receive,endCall;
    sock = socket(AF_INET , SOCK_DGRAM , 0);
    if (sock == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");
     memset((char *) &client, 0, sizeof(client));
    client.sin_family = AF_INET;
    client.sin_port = htons( SOCK_ADD );
 
    if (inet_aton(SERVER , &client.sin_addr) == 0) 
    {
        fprintf(stderr, "inet_aton() failed\n");
        exit(1);
    }
    printf("\n Enter \\q to end the call");

    static const pa_sample_spec ss = {
        .format = PA_SAMPLE_S16LE,
        .rate = 7000,
        .channels = 2
    };
    
    
    pa_simple *s = NULL;
    pthread_create(&receive, NULL, receive1, NULL);
    pthread_create(&endCall, NULL, endCall1, NULL);

    /* Create the recording stream */
    if (!(s = pa_simple_new(NULL, "App", PA_STREAM_RECORD, NULL, "record", &ss, NULL, NULL, &error))) {
        fprintf(stderr, __FILE__": pa_simple_new() failed: %s\n", pa_strerror(error));
        if (s)
            pa_simple_free(s);
    }
    while(1) 
    {
        uint8_t buf[BUFSIZE];
        /* Record some data ... */
        if (pa_simple_read(s, buf, sizeof(buf), &error) < 0)
         {
            fprintf(stderr, __FILE__": pa_simple_read() failed: %s\n", pa_strerror(error));
            if (s)
                pa_simple_free(s);
        }
        /* And write it to STDOUT */
        if (loop_write(sock, buf, sizeof(buf)) != sizeof(buf))
         {
            fprintf(stderr, __FILE__": write() failed: %s\n", strerror(errno));
            if (s)
                pa_simple_free(s);
        }
        if(dropCall == 1)
            break;
    }
    
    
    close(sock);
    return 0;
}

