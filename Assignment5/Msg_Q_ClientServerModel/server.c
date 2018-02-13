// This program accepts two inputs , adds it and replies client with result
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#define MAXSIZE     128
#define MAX_COUNT 5 //maximum degits used in arithmatic
void die(char *s)
{
  printf("in die ");
  perror(s);
  exit(1);
}

int add(char *s)
{
  char *a,*t;
  int l=0,i=0;
  int n,sum=0;
  a=strdup(s); //duplicate
  while(strlen(a)-1)
  {
		a = strtok(a, " "); //gives string upto where ' ' is found
		n=atoi(a);
		printf("a=%s",a);
		l+=strlen(a); //we save the length
		sum+=n;
		strncpy(a,s+l,strlen(s)+1-l); //updating the same pointer with rest part
		
		i++; //counting number of ' 's
  }
  printf("\n Sum = %d\n ", sum);
  return sum;
}


typedef struct msgbuf
{
	long    mtype;
	char    mtext[MAXSIZE];
} ;


int main()
{
	int msqid,sum;
	key_t key;
	struct msgbuf rcvbuffer;
	struct msgbuf sbuffer;
	size_t buflen;

	key = 1234;

	if ((msqid = msgget(key, 0666)) < 0)
		die("msgget");
		

	//Receive an answer of message type 1.
	if (msgrcv(msqid, &rcvbuffer, MAXSIZE, 1, 0) < 0)
		die("msgrcv");
	char a[12];
	sum = add(rcvbuffer.mtext);
	sbuffer.mtype = 1;
	//printf("Int sum %d",sum);	
	snprintf(sbuffer.mtext,MAX_COUNT,"%d",sum); //3 is hardcoded for sample
	printf("Buffer Sum = %s",sbuffer.mtext);
	buflen = strlen(sbuffer.mtext) + 1 ;
	if (msgsnd(msqid, &sbuffer, buflen, IPC_NOWAIT) < 0)
	{
		printf ("%d, %d, %s, %d\n", msqid, sbuffer.mtype, sbuffer.mtext, buflen);
		die("msgsnd");
	}

	else
		printf("Result Sent\n");
	//exit(0);
}
