#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include "biquad.h"
int CountPeaks(int a[],int);

int main()
{
   FILE *fp,*fd;

	char ch, buffer[32];
	int i = 0, j = 0;
    double dbGain=1;
    double  freq=30;
    double srate=150;
    double bandwidth=100;
    double arr[20000];
   //printf("Enter the name of file you wish to see\n");
   //gets(file_name);

   fp = fopen("Samples.csv","r"); // read mode
    fd=fopen("FilteredSamples.csv","w");
  	while(1)
	{
		ch = fgetc(fp);
		if(ch == EOF){
			break;
		}
		else if(ch == '\n')
		{

		    arr[j] = atof(buffer);
			j++;
			bzero(buffer, 32);


			i = 0;

			continue;
		}
		else{

			// reads the current character in the buffer
			buffer[i] = ch;
			i++;
		}
	}

    //for(i = 0; i < j; i++){
	//printf("%lf\n", arr[i]);
	//}
    double result[20000];
    //int type = 4;
    double a;
    for(int i=0;i<j;i++)
    {
    //a=double(arr[i]);
    result[i]=BiQuad(arr[i], BiQuad_new(dbGain,  freq, srate,  bandwidth));
     printf("%lf \n",result[i] );
     fprintf(fd,"\n%lf",result[i]);
    }

//	int peaks = CountPeaks(int arr[0],fs);

	fclose(fp);
	fclose(fd);
	return 0;

}
//int CountPeaks(int arr[],int fs)
//{
//int peakCount;
//return peakCount;

//}
double BiQuad(double sample, biquad * b)
{
    double result;

    /* compute result */
    result = b->a0 * sample + b->a1 * b->x1 + b->a2 * b->x2 -b->a3 * b->y1 - b->a4 * b->y2;

    /* shift x1 to x2, sample to x1 */
    b->x2 = b->x1;
    b->x1 = sample;

    /* shift y1 to y2, result to y1 */
    b->y2 = b->y1;
    b->y1 = result;

    return result;
}

/* sets up a BiQuad Filter */
biquad *BiQuad_new(double dbGain, double freq,double srate, double bandwidth)
{
    biquad *b;
    double A, omega, sn, cs, alpha, beta;
    double a0, a1, a2, b0, b1, b2;

    b = malloc(sizeof(biquad));
    if (b == NULL)
        return NULL;

    /* setup variables */
    A = pow(10, dbGain /40);
    //printf("\n%lf\n\n\n",freq /srate);
    omega = 2 * M_PI * freq /srate;
    sn = sin(omega);
    cs = cos(omega);
    alpha = sn * sinh(M_LN2 /2 * bandwidth * omega /sn);
    //printf("\n%lf\n\n\n",alpha);
    beta = sqrt(A + A);

//LPF
      b0 = (1 - cs) /2;
        b1 = 1 - cs;
        b2 = (1 - cs) /2;
        a0 = 1 + alpha;
        a1 = -2 * cs;
        a2 = 1 - alpha;

    /* normalising coefficients(a0=1) */
    b->a0 = b0 /a0;
    b->a1 = b1 /a0;
    b->a2 = b2 /a0;
    b->a3 = a1 /a0;
    b->a4 = a2 /a0;

    /* zero initial samples */
    b->x1 = b->x2 = 0;
    b->y1 = b->y2 = 0;

    return b;
}
