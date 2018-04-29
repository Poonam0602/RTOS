/***
SERVER
***/
/*command to compile:
cc -o server server.c -lpulse -lpulse-simple -lpthread
*/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <pulse/simple.h>
#include <pulse/error.h>
#include<pthread.h>
#include<arpa/inet.h> //inet_addr
#include<string.h>
#define BUFSIZE 1024
#define MESSAGE_BUFFER 1024
#define SOCK_ADD 8887

struct sockaddr_in server;
int length = sizeof(server);
int socket_desc;
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

/* A simple routine calling UNIX write() in a loop */
static ssize_t loop_write(int fd, const void*data, size_t size) {

    //printf("\nIn Loopwrite");
    ssize_t ret = 0;
    while (size > 0) {
        ssize_t r;
        if ((r = sendto(socket_desc, data, size , 0 , (struct sockaddr *) &server, length))<=0)
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
        if ((r = recvfrom(socket_desc, buf, BUFSIZE, 0, (struct sockaddr *) &server, &length)) <= 0) 
        {
            if (r == 0) /* EOF */
                break;
            fprintf(stderr, __FILE__": read() failed: %s\n", strerror(errno));
            if (s)
                pa_simple_free(s);
        }
        /* ... and play it */
        //printf("\n playing");
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
    pthread_t receive,endCall;

    socket_desc = socket(AF_INET , SOCK_DGRAM , 0);
    if (socket_desc == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");
     
    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( SOCK_ADD );
     
    //Bind
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        //print the error message
        perror("bind failed. Error");
        return 1;
    }
    puts("bind done");
     
    printf("\n Enter \\q to end the call");

    static const pa_sample_spec ss = {
        .format = PA_SAMPLE_S16LE,
        .rate = 7000,
        .channels = 2
    };
    int ret = 1;
    pa_simple *s = NULL;
    int error;

    pthread_create(&receive,NULL,receive1,NULL);
    pthread_create(&endCall, NULL, endCall1, NULL);

     
    /* Create the recording stream */
    
    if (!(s = pa_simple_new(NULL, "App", PA_STREAM_RECORD, NULL, "record", &ss, NULL, NULL, &error))) {
        fprintf(stderr, __FILE__": pa_simple_new() failed: %s\n", pa_strerror(error));
        if (s)
            pa_simple_free(s);
        return ret;
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
            return ret;
        }
        /* And write it to STDOUT */
        if (loop_write(socket_desc, buf, sizeof(buf)) != sizeof(buf))
         {
            fprintf(stderr, __FILE__": write() failed: %s\n", strerror(errno));
            if (s)
                pa_simple_free(s);
            return ret;
        }
        if(dropCall == 1)
            break;
    }

    close(socket_desc);
    return 0;

}
