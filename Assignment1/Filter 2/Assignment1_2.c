#include<stdio.h>
#include<stdlib.h>
#include<math.h>
int main()
{
    FILE *fpr, *fpw;
    fpr = fopen("Sample.csv","r");
    fpw = fopen("FilteredSamples.csv","w+");
    char s[10];
    char ch;
    double sampleT, sampleV;
    double fs = 120;
    double fc = 30;

    double pi = 3.14;
    double b0,b1,b2,a1,a2;
    double ff = fc/fs;
    double ita = 1/ tan(pi*ff);
    double q = sqrt(2);
    b0 = 1.0 / (1.0 + (q*ita) + (ita*ita));
    b1 = 2*b0;
    b2= b0;
    a1 = 2.0 * (ita*ita - 1.0) * b0;
    a2 = -(1.0 - q*ita + ita*ita) * b0;

    double x0,x1,x2,y1,y2,y;
    fscanf(fpr,"%s",s);
    x1=0;
    x2=0;
    y1=0;
    y2=0;
    int n=0;

    while(!feof(fpr))
    {
	    fscanf(fpr,"%lf",&x0);
	    y = b0*x0 + b1*x1 + b2*x2 + a1*y1 + a2*y2;
	    if(n == 0)
	    {
		    x1 = x0;
		    y1 = y;
		    x2 = 0;
		    y2 = 0;
		    n = n + 1;
	    }
	    else
	    {
		    x1 = x0;
		    y1 = y;
		    x2 = x1;
		    y2 = y1;
	    }
	    fprintf(fpw,"%f\n",y);
    }
    fclose(fpr);
    fclose(fpw);
    return 0;
}

