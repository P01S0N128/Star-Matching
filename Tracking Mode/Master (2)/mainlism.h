// #include <stdio.h>
#include "cortos.h"
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "sm_gnrt_3D_vecs.h"
#include "sm_4_star_main.h"
#include "sm_4_star_circulate.h"
#include "bubblesort.h"
#include "sm_constants.h"
#include "sm_validation.h"

#include "sm_K_vec_arr.h"
#include "sm_GC.h"


int sm(long double UIS[][3], int N_i, long double body_vecs_IS[][4], int sm_IS[][2])
{
    // uint64_t t1 = cortos_get_clock_time();
    int i, countt, j, k; //Declaring counter variables
    // inputs/constants---------------------------------------

    //int N_i, N_uis, N_max, N_th, N_gc, N_kvec_pairs, N_circ = 0, N_is = 0;
    int N_uis, N_max, N_th, N_gc, N_kvec_pairs, N_circ = 0, N_is = 0;
    long double epsilon, q, m, foc, y_max, y_min, delta;

    // -------------------------------------------------------
    // Value of hyperparameters

    epsilon = EPSILON;
    foc = FOCAL_LENGTH;
    delta = DELTA;
    
    // Some other constants
    // y_max = 0.999999999992621;
    y_max = 0.9999999999926209;
    y_min = 0.990026120824787;
    // y_min = 0.973988966620749;
    N_kvec_pairs = 224792;
    // N_kvec_pairs = 188807;
    N_gc = 8876;
    // N_gc = 5060;

    // Inputs - will come from FE block. 
    //printf("Enter the number of stars in input test file :- ");
    //scanf("%d", &N_i);
    N_i = 26;
    N_uis = N_i;
    //printf("Enter the maximum number of iterations (< no. of input stars) you want the algorithm to run :- ");
    //scanf("%d", &N_max);
    N_max = 25;
    //printf("Enter the maximum number of matched stars you want :- ");
    //scanf("%d", &N_th);
    N_th = 25;

    //Constants for validation
    double tol = TOL;
    double p_1 = P1;
    double p_2 = P2;

    //--------------------------------------------------------
    // constants for using the k vector table (to be used in the 4 star matching)

    m = (y_max - y_min + 2 * epsilon) / (N_kvec_pairs - 1);
    q = y_min - m - epsilon;

    // printf("M is %0.15Lf\n", m);
    // printf("Q is %0.15Lf\n", q);
    
    // -------------------------------------------------------------------------------------------------------
    //int sm_IS[N_gc][2]; // array for storing the matched stars
    //long double body_vecs_IS[N_gc][4]; //Array for storing corresponding body frame vectors for matched stars
    memset(sm_IS, -1, N_gc * sizeof(sm_IS[0]));
    memset(body_vecs_IS, -1, N_gc * sizeof(body_vecs_IS[0]));

    // sorting the UIS table according to Euclidean distance
    // bubbleSort(UIS, N_i);

    // for (int i = 0; i < N_i; i++)
    // {
    //     for (int j = 0; j < 3; j++)
    //     {
    //         printf("%Lf", UIS[i][j]);
    //     }
    //     printf("\n");
    // }


    long double sm_3D_vecs[N_i][4]; // this stores the 3D vectors generated from the UIS table
    // generating 3D vectors from the sorted UIS table
    sm_gnrt_3D_vec(sm_3D_vecs, UIS, foc, N_i);

    // main algo starts here
    int circ_flag = 1; // flag which stores the number of times the sm_3D_vecs table has been circulated
    for (i = 1; i <= N_max; i++)
    {
       
        if (N_uis >= 4 && N_is < N_th) // N_uis >= 4 for running 4 star algo & N_is(identified stars) < N_th(threshold number of matched stars)
        {
            int N_match = 0; // variable for storing the number of stars matched in a particular iteration
            double four_stars[4][4]; // this will store the extracted 4 stars from the sm_3D_vecs table
            //for (int countt = 0, j = 0; j < N_i && countt < 4; j++) // here the variable countt is used just to count whether 4 stars have been extracted
            for (countt = 0, j = 0; j < N_i && countt < 4; j++)
            {
                if ((int)sm_3D_vecs[j][0] != -1)
                {

                    //for (int k = 0; k < 4; k++)
                    for (k = 0; k < 4; k++)
                    {
                        four_stars[countt][k] = sm_3D_vecs[j][k];
                    }
                    countt++;
                }
            }
            sm_4_star(four_stars, sm_3D_vecs, sm_IS, body_vecs_IS, sm_K_vec_arr, &N_match, N_i, N_gc, delta, q, m);
            N_uis -= N_match;
            N_is += N_match;

            // printf("%d\n", N_match); // DEBUG line

            if (N_match == 0 && N_circ <= 2 * N_i) // if no stars are matched then we must circulate the sm_3D_vecs table
            {
                sm_4_star_circulate(sm_3D_vecs, &N_circ, N_i);
                if (N_circ >= 2 * N_i)
                {
                    break;  // end the loop after checking this condition, as mentioned in the flowchart
                }
            }
        }
        else
        {
            // end the loop if the conditions "N_uis >= 4 && N_is < N_th" are not satisfied
            break;
        }
    }
    printf(" \n Before Verification, Total matched stars :- %d\n\n", N_is);
    printf("  Input_ID  Desired_star_ID  X             Y             Z          X_b         Y_b         Z_b\n");
    printf("-----------------------------------------------------------------------------------------------------\n");
    //for (int i = 0; i < N_gc; i++)
    //for (i = 0; i < N_gc; i++)
    int c=0;
    for (i = 0; i < N_i; i++)
    {
        if ((int)sm_IS[i][0]!=-1)
        {
            c++;
            printf("%d     %d      %d         ", i, sm_IS[i][0], sm_IS[i][1]);
            //for (int j = 1; j < 4; j++)
            for (j = 1; j < 4; j++)
            {
                printf("%lf    ", sm_GC[(int)sm_IS[i][1] - 1][j]);
            }
            for (j = 1; j < 4; j++){
                printf("%Lf     ", body_vecs_IS[i][j]);
            }
            printf("\n");
        }
    }
    printf("----------------------------------------------------------------------------------------------------\n");
    sm_validate(sm_3D_vecs, sm_IS, body_vecs_IS, sm_GC, &N_is, N_i, N_gc, tol, p_1, p_2);
    printf(" \nAfter verification, Total matched stars :- %d\n\n", N_is);
    printf("  Input_ID  Desired_star_ID  X             Y             Z          X_b         Y_b         Z_b\n");
    printf("----------------------------------------------------------------------------------------------------\n");
    //for (int i = 0; i < N_gc; i++)
    //for (i = 0; i < N_gc; i++)
    for (i = 0; i < N_i; i++)
    {
        if ((int)sm_IS[i][0]!=-1)
        {
            printf("%d     %d      %d         ", i, sm_IS[i][0], sm_IS[i][1]);
            //for (int j = 1; j < 4; j++)
            for (j = 1; j < 4; j++)
            {
                printf("%lf    ", sm_GC[(int)sm_IS[i][1] - 1][j]);
            }
            for (j = 1; j < 4; j++){
                printf("%Lf     ", body_vecs_IS[i][j]);
            }
            printf("\n");
        }
    }
    return c;
}