//#include <stdio.h>
#include "cortos.h"
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "sm_gnrt_3D_vecs.h"
#include "sm_4_star_main.h"
#include "sm_4_star_circulate.h"
#include "bubblesort.h"

#include "UIS.h"
#include "sm_K_vec_arr.h"
#include "sm_GC.h"

//int sm_K_vec_arr[188807][3]; // declared here because array of such sizes can't be declared inside main() in C

int main()
{
    // inputs/constants---------------------------------------

    int N_i, N_uis, N_max, N_th, N_gc, N_kvec_pairs, N_circ = 0, N_is = 0;
    int N_match = 0;
    double four_stars[4][4];
    double epsilon, q, m, foc, y_max, y_min, delta;
    int i = 0, j = 0, countt = 0, k = 0;
    // -------------------------------------------------------
    // Value of hyperparameters
    y_max = 0.999999999992621;
    y_min = 0.973988966620749;
    epsilon = 2.22e-15;
    foc = 0.025;
    delta = 5e-5;
    
    // Some other constants
    N_kvec_pairs = 188807;
    N_gc = 5060;

    // Inputs
    //CORTOS_INFO("Enter the number of stars in input test file :- ");
    //scanf("%d", &N_i);
    N_i = 25;
    N_uis = N_i;
    //CORTOS_INFO("Enter the maximum number of iterations (< no. of input stars) you want the algorithm to run :- ");
    //scanf("%d", &N_max);
    N_max = 24;
    //CORTOS_INFO("Enter the maximum number of matched stars you want :- ");
    //scanf("%d", &N_th);
    N_th = 2;
    //--------------------------------------------------------
    // taking input of the K vector catalogue
    //FILE *file;
    //file = fopen("kvec.txt", "r");
    //for (int i = 0; i < N_kvec_pairs; i++)
    //for (i; i < N_kvec_pairs; i++)
    //{
        //for (int j = 0; j < 3; j++)
    //    for (j; j < 3; j++)
    //    {
    //        int temp;
    //        fscanf(file, "%d", &temp);
    //        sm_K_vec_arr[i][j] = temp;
    //    }
    //}
    //i = 0;
    //j = 0;
    //fclose(file);

    // taking input of test file
    //double UIS[N_i][3];     // 2D array for storing (x,y) coordinates and star IDs of unidentified stars
    //FILE *file2;
    //file2 = fopen("req1.txt", "r"); // type the name of your input test file here
    //for (int i = 0; i < N_i; i++)
    //for (i; i < N_i; i++)
   // {
        //for (int j = 0; j < 3; j++)
    //    for (j; j < 3; j++)
   //     {
    //        double temp;
   //         fscanf(file2, "%lf", &temp);
   //         UIS[i][j] = temp;
   //     }
   // }
   // i = 0;
   // j = 0;
   // fclose(file2);

    // taking input of Guide star catalogue
    //double sm_GC[5060][4];
    //FILE *file3;
   // file3 = fopen("gsc.txt", "r"); 
    //for (int i = 0; i < N_gc; i++)
    //for (i; i < N_gc; i++)
    //{
        //for (int j = 0; j < 4; j++)
    //    for (j; j < 4; j++)
    //    {
    //        double temp;
   //         fscanf(file3, "%lf", &temp);
    //        sm_GC[i][j] = temp;
   //     }
  //  }
  //  i = 0;
 //   j = 0;
 //   fclose(file3);

    //--------------------------------------------------------
    // constants for using the k vector table (to be used in the 4 star matching)

    m = (y_max - y_min + 2 * epsilon) / (N_kvec_pairs - 1);
    q = y_min - m - epsilon;
    
    // -------------------------------------------------------------------------------------------------------
    int sm_IS[N_gc][2]; // array for storing the matched stars
    memset(sm_IS, -1, N_gc * sizeof(sm_IS[0]));
    //CORTOS_DEBUG("114");

    // sorting the UIS table according to Euclidean distance
    bubbleSort(UIS, N_i);
    //CORTOS_DEBUG("118");

    double sm_3D_vecs[N_i][4]; // this stores the 3D vectors generated from the UIS table
    // generating 3D vectors from the sorted UIS table
    sm_gnrt_3D_vec(sm_3D_vecs, UIS, foc, N_i);
   // CORTOS_DEBUG("119");

    // main algo starts here
    int circ_flag = 1; // flag which stores the number of times the sm_3D_vecs table has been circulated
    //for (int i = 1; i <= N_max; i++) // N_max is the maximum number of times we want to run the code
    i = 1;
    for (i; i <= N_max; i++) // N_max is the maximum number of times we want to run the code
    {
        //CORTOS_DEBUG("131");
        if (N_uis >= 4 && N_is < N_th) // N_uis >= 4 for running 4 star algo & N_is(identified stars) < N_th(threshold number of matched stars)
        {
            //int N_match = 0; // variable for storing the number of stars matched in a particular iteration
            N_match = 0;
            //CORTOS_DEBUG("138");
            //double four_stars[4][4]; // this will store the extracted 4 stars from the sm_3D_vecs table
            //for (int countt = 0, j = 0; j < N_i && countt < 4; j++) // here the variable countt is used just to count whether 4 stars have been extracted
            for (countt, j; j < N_i && countt < 4; j++) // here the variable countt is used just to count whether 4 stars have been extracted
            {
               // CORTOS_DEBUG("143");
                if ((int)sm_3D_vecs[j][0] != -1)
                {

                    for (k; k < 4; k++)
                    {
                        four_stars[countt][k] = sm_3D_vecs[j][k];
                    }
                    countt++;
                }
            }
            sm_4_star(four_stars, sm_3D_vecs, sm_IS, sm_K_vec_arr, &N_match, N_i, N_gc, delta, q, m);
           // CORTOS_DEBUG("155");
            N_uis -= N_match;
            N_is += N_match;
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
   // CORTOS_DEBUG("167");
    i = 0;
    j = 0;
    k = 0;
    CORTOS_INFO(" \nTotal matched stars :- %d\n\n", N_is);
    CORTOS_INFO("  Input_ID  Desired_star_ID  X             Y             Z\n");
    CORTOS_INFO("-------------------------------------------------------------------\n");
    //for (int i = 0; i < N_gc; i++)
    for (i; i < N_gc; i++)
    {
        if (sm_IS[i][0]!=-1)
        {
            CORTOS_INFO("%d     %d      %d         ", i, sm_IS[i][0], sm_IS[i][1]);
            //for (int j = 1; j < 4; j++)
            j = 1;
            for (j; j < 4; j++)
            {
                CORTOS_INFO("%lf    ", sm_GC[sm_IS[i][1]][j]);
            }
            CORTOS_INFO("\n");
        }
    }
    cortos_exit(0);
}
