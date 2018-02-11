/*Problem -1

We have standard linux utilties ls , grep, wc etc

Write a C program that collect the output of one of these and pipe it as input into another – Do not use standard | operator
*/

#include<unistd.h>
#include<stdio.h>
#include<sys/types.h>
#include<fcntl.h>
#include <sys/wait.h>

int main()
{
	int fd[2];
	pipe(fd);
	pid_t i;
	int status;
	i=fork();
	printf("pid = %d",i);	
	if(!i)
	{
		printf("pid1 = %d",getpid());		
		printf("ls start");		
		close(1);//disable stdout
		close(fd[0]);//disable read end of pipe
		dup(fd[1]);//stdout <-- fd[1]
		//execlp("ls","ls","-l",(char*)NULL);
		execl("//bin//ls","ls",NULL);
		printf("ls excuted");
	}
	else
	{
		waitpid (i, &status, 0);		
		printf("pid2 = %d",getpid());		
		printf("wc start ");		
		close(fd[1]);//disable write end of pipe
		close(0);//stdin disable
		dup(fd[0]);//stdin <-- fd[0]
		execlp("wc","wc","-l",(char*)NULL);
		//execl("//usr//bin//ls","wc","-l",NULL);
		printf("wc executed");
	}
} 
