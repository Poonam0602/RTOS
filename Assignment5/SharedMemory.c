// To demonstrate data sharing between two processes using shared memory

#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>	// shared memory
#include <sys/types.h>		// for wait()
#include <sys/wait.h>		// for wait()

main()
{
	int shmid, status;
	int *a, *b;
	int i;

	// shmget() returns a shared memory
	shmid = shmget(IPC_PRIVATE, 2*sizeof(int), 0777|IPC_CREAT);

	/*The parent and child must "attach" the shared memory to its local data segment. This is done by the shmat()
	    system call. shmat() takes the SHMID of the shared memory segment as input parameter and returns the address at which
	    the segment has been attached. Thus shmat() returns a char pointer.	*/
	    
	if (fork() == 0) {

		/* Child Process */

		/* The address is stored in the int pointer b. */
		b = (int *) shmat(shmid, 0, 0);

		for( i=0; i< 10; i++) {
			sleep(1);
			printf("\t\t\t Child reads: %d\n",*b);
		}
		
		/* Detach child from the shared memory after it is used */

		shmdt(b);

	}
	else {

		/* Parent Process */

		a = (int *) shmat(shmid, 0, 0); //Memory is shared using shmat

		*a = 0; 
		for( i=0; i< 10; i++) {
			sleep(1);
			*a = *a + 10;
			printf("Parent writes: %d\n",*a);
		}
		wait(&status);

		shmdt(a);
		shmctl(shmid, IPC_RMID, 0);//Delete shared memory
	}
}

