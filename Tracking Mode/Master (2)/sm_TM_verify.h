// #include <stdio.h>
#include "cortos.h"
#include <stdlib.h>
#include <math.h>

//#include "sm_TM_calc_angdist.h"
;
int verify(double final[][3], int len, double finalmat[][3], double error)
{
    int count = 0;
    int i=0, j=0;
    double d1 = 0, d2 = 0, d=0;
    int okay = 1;
    for (i=0; i<len; i++)
    {
        okay = 1;
        for (j=i+1; j<len; j++)
        {
            d1 = sm_TM_id_angdist(final[i][0], final[j][0], sm_GC);
            d2 = sm_TM_centroid_angdist(final[i][1], final[i][2], final[j][1], final[j][2], 0.036, 1);
            d = d1 - d2;
            if ((d>error) || (d<-error)) okay = 0;
        }

        if (okay == 1)
        {
            finalmat[count][0] = final[i][0];
            finalmat[count][1] = final[i][1];
            finalmat[count][2] = final[i][2];
            count++;
        }
    }
    return count;
}