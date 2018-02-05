/*Objective : Experiment with Address space of a process
The main program

    void main()
    {
    }

will create Three child process.

What the main program will do is
1. print the address of three local variable int a b c .
2. Then it will print address of three global variable   int g_first , g_second, g_third
3. In third third step it will fork() three child process and parent enter into a infinite loop and wait for ever (Do not terminate)
4. Each of these process will execute a function PrintVariable(). Parent does not call this function instead it will go into infinite loop
5. The function PrintVariable will print the address of the three local int j,k,l ;  which are declared inside the function PrintVariable()
6. After this the same function will also print the address of the global variable  int g_first , g_second, g_third
7. After this all the three child process will get into a infinite loop just like parent and wait for ever

Observe the value of the address printed by each of the child process for each of the variable and document it in the form of a table.
Run this program again and again and document the address seen.
*/

#include <unistd.h>
#include <sys/types.h>
#include<string.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <stdlib.h>


int g1=50,g2=51,g3=52;   //Global Variables
void printVariable()
{
	int a,b,c;      //Local Variables
	a=1;
	b=2;
	c=3;
        printf("\nIn PrintVariable Function ::\n Local Address: \t%p => %d \t%p => %d \t%p =>%d\n",(void*)&a,a,(void*)&b,b,(void*)&c,c);
	printf("\nIn PrintVariable Function ::\nGlobal address : \t%p =>%d \t%p => %d \t%p => %d\n",(void*)&g1,g1,(void*)&g2,g2,(void*)&g3,g3);
	return;
}
int main()
{
	pid_t childPID1,childPID2,childPID3;
        int a,b,c,pid,*num;      //Local Variables
	a=100;
	b=200;
	c=300;	
        
	printf("\n Local Address: \t%p => %d \t%p => %d \t%p =>%d\n",(void*)&a,a,(void*)&b,b,(void*)&c,c);
        printf("\nGlobal address : \t%p =>%d \t%p => %d \t%p => %d\n",(void*)&g1,g1,(void*)&g2,g2,(void*)&g3,g3);

	childPID1 = fork();
	if(childPID1 >= 0) // fork was successful
	{
		if(childPID1 == 0) // child process
		{
			printf("\n\n Child 1: \n");
			++a;++b;++c;			
			printf("\n Local Address: \t%p => %d \t%p => %d \t%p =>%d\n",(void*)&a,a,(void*)&b,b,(void*)&c,c);
			printf("\nGlobal address : \t%p =>%d \t%p => %d \t%p => %d\n",(void*)&g1,++g1,(void*)&g2,++g2,(void*)&g3,++g3);
			printf("\n ChildPID1 = %d,ChildPPID1 = %d \n\n ",getpid(),getppid());					
			printVariable();

			
			while(1);
		}
		else //Parent process
		{
			childPID2 = fork();
			if(childPID2 >= 0) // fork was successful
			{
				if(childPID2 == 0) // child process
				{
					sleep(2);					
					printf("\n\n Child 2: \n");
					++a;++b;++c;					
					printf("\n Local Address: \t%p => %d \t%p => %d \t%p =>%d\n",(void*)&a,a,(void*)&b,b,(void*)&c,c);
					printf("\nGlobal address : \t%p =>%d \t%p => %d \t%p => %d\n",(void*)&g1,--g1,(void*)&g2,--g2,(void*)&g3,--g3);			
					printf("\n ChildPID2 = %d, ChildPPID1 = %d \n\n",getpid(),getppid());	
					printVariable();
					
					while(1);
				}
				else
				{
					childPID3=fork();
					if(childPID3 >= 0) // fork was successful
					{
						if(childPID3 == 0) // child process
						{
							sleep(4);							
							printf("\n\n\n Child 3: \n");
							a=a+2;b=b+2;c=c+2;
							printf("\n Local Address: \t%p => %d \t%p => %d \t%p =>%d\n",(void*)&a,a,(void*)&b,b,(void*)&c,c);
							printf("\nGlobal address : \t%p =>%d \t%p => %d \t%p => %d\n",(void*)&g1,g1+3,(void*)&g2,g2+3,(void*)&g3,g3+3);
							
						printf("\n ChildPID3 = %d , ChildPPID1 = %d \n\n",getpid(),getppid());
						printVariable();
							
							while(1);
						}
						else
						{
							
							sleep(6);							
							printf("\n\n\n Parent: \n");
							a=a+1000;b=b+1000;c=c+1000;
							printf("\n Local Address: \t%p => %d \t%p => %d \t%p =>%d\n",(void*)&a,a,(void*)&b,b,(void*)&c,c);							
							printf("\n parentPID = %d \n\n ",getpid());
							printf("\nGlobal address : \t%p =>%d \t%p => %d \t%p => %d\n",(void*)&g1,g1,(void*)&g2,g2,(void*)&g3,g3);							
							int fd;	
							fd = open("/media/poonam/Stuff/IIITB Assignments/RTOS/Assignment 2/killProcessFD.sh", O_CREAT|O_TRUNC|O_WRONLY, 0666);
							char buf[100];char buf1[2];
							strcpy(buf, "#!/bin/bash\n");
							strcat(buf,"kill -9 ");
							
							sprintf(buf1,"%d\n",childPID1);
							strcat(buf,buf1);

							strcat(buf,"kill -9 ");
							
							sprintf(buf1,"%d\n",childPID2);
							strcat(buf,buf1);

							strcat(buf,"kill -9 ");
							
							sprintf(buf1,"%d\n",childPID3);
							strcat(buf,buf1);
							strcat(buf,"kill -9 ");
							int parentpid=getpid();
							sprintf(buf1,"%d\n",parentpid);
							strcat(buf,buf1);
							
int n=strlen(buf);
							//printf("\n\n\n%s",buf);
							write(fd, buf, n);  	

							close(fd);
							printVariable();
							wait(NULL);	//helps not to create zombie processes.								
							while(1);
							//{printf("\n Parent..");}
						}
					}
					else{printf("\n\n\n Fork failed : Child 3");}

				}
			}
			else{printf("\n\n\n Fork failed : Child 2");}

		}
	}
	else // fork failed
	{
		printf("\n Fork failed, quitting!!!!!!: Child 1\n");
		return 1;
	}
	return 0;
	
}
