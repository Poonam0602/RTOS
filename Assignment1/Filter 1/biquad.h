#include <math.h>
#include <stdlib.h>

#ifndef M_LN2
#define M_LN2	   0.69314718055994530942
#endif

#ifndef M_PI
#define M_PI		3.14159265358979323846
#endif

/* this holds the data required to update samples thru a filter */
typedef struct {
    double a0, a1, a2, a3, a4;
    double x1, x2, y1, y2;
}
biquad;

extern double BiQuad(double sample, biquad * b);
extern biquad *BiQuad_new(double dbGain, /* gain of filter */
                          double freq,             /* center frequency */
                          double srate,            /* sampling rate */
                          double bandwidth);       /* bandwidth in octaves */
