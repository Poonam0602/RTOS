#include<stdio.h>
#include<stdlib.h>
#include<math.h>
int main()
{
    FILE *fpr, *fpw;
    fpr = fopen("Samples.csv","r");
    fpw = fopen("FilteredSamples.csv","w");
    char s[10];
    char ch;
    double sampleT, sampleV;
    double fs = 150;
    double fc = 40;

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
    fscanf(fpr,"%s",&s);
    x1=0;
    x2=0;
    y1=0;
    y2=0;
    int n=0;

        while(ch != EOF)
        {
            fscanf(fpr,"%lf",&sampleT);
            fprintf(fpw,"%lf",sampleT);
            ch=fgetc(fpr);
            fputc(ch,fpw);
            fscanf(fpr,"%lf",&sampleV);
            x0 = sampleV;
            y = (a1*y1)+(a2*y2)+(b0*x0)+(b1*x1)+(b2*x2);
            printf("%lf",y);
            fprintf(fpw,"%lf", y);
                if(n==1)
                {
                    x2=0;
                    x1=x0;
                    y2=0;
                    y1=y;
                }
                else
                {
                    x2=x1;
                    x1=x0;
                    y2=y1;
                    y1=y;
                }
            ch=fgetc(fpr);
            fputc(ch,fpw);
            printf("\n");
            n = n+1;
        }
    fclose(fpr);
    fclose(fpw);
    return 0;
}


