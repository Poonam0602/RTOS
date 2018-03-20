#include <errno.h>
#include <stdio.h>
#include <wiringPi.h>
#include <string.h>

#define	INTR	2  

void ISR(void)
{
	printf("Received Value is : %d\n",(8*digitalRead(0) + 4*digitalRead(7) + 2*digitalRead(9) + digitalRead(8)));

}
int main (void)
{
    printf ("Raspberry Pi blink\n") ;
      if (wiringPiSetup () == -1)
        return 1 ;
    pinMode (8, INPUT) ;//lsb
    pinMode (9, INPUT) ;
    pinMode (7, INPUT) ;
    pinMode (0, INPUT) ;//msb
    pinMode (INTR,INPUT);

    //condition to trigger interrupt service routine on Falling Edge
	if(wiringPiISR(digitalRead(INTR),INT_EDGE_FALLING,&ISR)<0)
	{
		fprintf(stderr,"Unable to setup ISR: %s\n",strerror(errno));
		return 1;
	}

	while(1){}
    return 0;
}

