#include <stdio.h>
#include <wiringPi.h>

int main (void)
{
printf ("Raspberry Pi blink\n") ;
 
  if (wiringPiSetup () == -1)
    return 1 ;
pinMode (8, INPUT) ;//lsb
pinMode (9, INPUT) ;
pinMode (7, INPUT) ;
pinMode (0, INPUT) ;//msb
int testVal = 21;

while(1)
{

/*//To set output
digitalWrite(7, LOW);
delay(1000);
digitalWrite(7, HIGH);
delay(1000);*/
int decVal;
int b0,b1,b3,b2;
b0=digitalRead(8);
b1=digitalRead(9);
b2=digitalRead(7);
b3=digitalRead(0);

decVal = (b3*8)+(b2*4)+(b1*2)+b0;
if(decVal != testVal)
{
printf("dec value: %d\n", decVal);
testVal = decVal;
}
}
return 0;
}
