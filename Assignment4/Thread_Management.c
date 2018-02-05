/*
Experiment with Address space of a thread
1. Write a program which will create three threads
2. There will be three global variables in the main program int a,b,c
3. Set the Value of the Global Variables in the mail program
4. Now Write Three functions Thread1Proc(), Thread2Proc(), Thread3Proc(), each of them should have local variables
      int i,j,k
5. Create three thread and each of the thread will execute the corresponding ThreadxProc() eg Thread1 will execute Thread1Proc()
6. What each ThreadProc() should do is print the address of both global and local variables then enter into a infinite loop and print the value of global and local variables
7. Change the values of the global variables in  each thread 
*/

#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>

pthread_t tid[3];
int a,b,c;
void Thread1Proc();
void Thread2Proc();
void Thread3Proc();

void* threadProcess(void *arg)
{
    //unsigned long i = 0;
    pthread_t id = pthread_self();

    if(pthread_equal(id,tid[0]))
    {
        printf("\n First thread processing\n");
	while(1)
		Thread1Proc();
    }
    else if(pthread_equal(id,tid[1]))
    {
	sleep(2);        
	printf("\n Second thread processing\n");
	while(1)
		Thread2Proc();
    }
    else
    {
	sleep(3);	 
	printf("\n Third thread processing\n");
	while(1);
		Thread3Proc();
    }

    //sleep(4);

    return NULL;
}

int main(void)
{
    int i = 0;
    int err;
    a=20;b=30;c=40;
    printf("PID = %d",getpid());
    while(i < 3)
    {
        err = pthread_create(&(tid[i]), NULL, &threadProcess, NULL);
        if (err != 0)
            printf("\ncan't create thread :[%s]", strerror(err));
        else
	{
            printf("\n Thread created successfully\n");
	}
        i++;
    }
    pthread_join(tid[0],NULL);
    pthread_join(tid[1],NULL);
    pthread_join(tid[2],NULL);	
    sleep(5);
    return 0;
}

void Thread1Proc()
{
   int i,j,k;
   i=2;j=3;k=6;
	printf("\n\n Thread 1: \n\n");
   printf("\n Global Address: \t%p => %d \t%p => %d \t%p =>%d\n",(void*)&a,a++,(void*)&b,b++,(void*)&c,c++);
   //printf("\n Local address : \t%p =>%d \t%p => %d \t%p => %d\n",(void*)&x,x,(void*)&y,y,(void*)&y,y);	
printf("\n Local address : \t%p =>%d \t%p => %d \t%p => %d\n",(void*)&i,i,(void*)&j,j,(void*)&k,k);			 

}
void Thread2Proc()
{
   int x,y,z;
   x=2;y=3;z=6;
   a+=10;b+=10;c+=10;
	printf("\n\n Thread 2: \n\n");
   printf("\n Global Address: \t%p => %d \t%p => %d \t%p =>%d\n",(void*)&a,a,(void*)&b,b,(void*)&c,c);
   printf("\n Local address : \t%p =>%d \t%p => %d \t%p => %d\n",(void*)&x,x,(void*)&y,y,(void*)&y,y);			 

}
void Thread3Proc()
{
   int x,y,z;
   x=2;y=3;z=6;
	printf("\n\n Thread 3: \n\n");
   printf("\n Global Address: \t%p => %d \t%p => %d \t%p =>%d\n",(void*)&a,--a,(void*)&b,--b,(void*)&c,--c);
   printf("\n Local address : \t%p =>%d \t%p => %d \t%p => %d\n",(void*)&x,x,(void*)&y,y,(void*)&y,y);			 

}
