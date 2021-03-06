//#include <stdio.h>
#include "cortos.h"
#include <stdlib.h>
#include <math.h>

void sm_gnrt_3D_vec(long double sm_3D_vecs[][4],long double sm_sorted_UIS[][3], double foc, int N_i){
    uint64_t t1 = cortos_get_clock_time();
    // this function is pretty straight-forward; directly the formula for generating 3D vectors has been applied
    int i; //Declaring counter variables
    //for (int i = 0; i < N_i; i++)
    for (i = 0; i < N_i; i++)
    {
        long double local = sqrt((sm_sorted_UIS[i][1] / foc) * (sm_sorted_UIS[i][1] / foc) + (sm_sorted_UIS[i][2] / foc) * (sm_sorted_UIS[i][2] / foc) + 1);
        sm_3D_vecs[i][0] = sm_sorted_UIS[i][0];
        sm_3D_vecs[i][1] = (sm_sorted_UIS[i][1] / (foc*local));
        sm_3D_vecs[i][2] = (sm_sorted_UIS[i][2] / (foc*local));
        sm_3D_vecs[i][3] = 1/local;
    }
    uint64_t t2 = cortos_get_clock_time();
    uint32_t t11 = t1&(0xffffffff);
    uint32_t t12 = t2&(0xffffffff);
    cortos_printf("\ntime required for generating 3-D vecs is %u \n", t12-t11);
}