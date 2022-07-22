// #include <stdio.h>
#include "cortos.h"
#include <stdlib.h>
#include <math.h>
#include "sm_TM_calc_angdist.h"

int sm_TM_SNT_match(double sm_TM_RBM_matchmat[][3],
                     int L,
                     double fe_output[][2],
                     int N,
                     int sm_TM_SNT[5060][11],
                     double sm_GD_SC[5060][4],
                     double sm_TM_CP_F,
                     double pixel_size,
                     int Nth,
                     double error,
                     double sm_TM_SNT_output[][3])
{
double fe_unmatched[N][2];
int num_unmatched = 0;

int j=0;
int i=0;
for (j=0; j < N; j++)
{
    int already_matched = 0;

    for (i=0; i < L; i++)
    {
        if ((sm_TM_RBM_matchmat[i][0] == fe_output[j][0]) && (sm_TM_RBM_matchmat[i][1] == fe_output[j][1]))
        {
            already_matched = 1;
            break;
        }
    }
    if (already_matched==0)
    {
        fe_unmatched[num_unmatched][0] = fe_output[j][0];
        fe_unmatched[num_unmatched][1] = fe_output[j][1];
        num_unmatched++;
    }
}

//cout<<"done1"<<endl<<endl;

int new_matched = 0, running = 0;
for (running = 0; running < num_unmatched; running++)
{
    int done = 0;
    for (i=0; i < L; i++)
    {
        //cout<<endl;
        if (new_matched == Nth - L) break;
        int curr_ref_star = sm_TM_RBM_matchmat[i][2];
        if (curr_ref_star > 5060) continue;
        j = 0;

        while (sm_TM_SNT[curr_ref_star-1][j] != 0)
        {
            double x1 = sm_TM_RBM_matchmat[i][0];
            double y1 = sm_TM_RBM_matchmat[i][1];
            double x2 = fe_unmatched[running][0];
            double y2 = fe_unmatched[running][1];

            double q1 = sm_TM_id_angdist((curr_ref_star-1), (sm_TM_SNT[curr_ref_star-1][j]-1), sm_GD_SC);
            double q2 = sm_TM_centroid_angdist(x1, y1, x2, y2, sm_TM_CP_F, pixel_size);
            double q = q1 - q2;

            if ((q<error) && (q>(-error)))
            {
                int new_id = sm_TM_SNT[curr_ref_star-1][j];
                for (int i=0; i<new_matched; i++)
                    if (sm_TM_SNT_output[i][2] == new_id) continue;

                sm_TM_SNT_output[new_matched][2] = sm_TM_SNT[curr_ref_star-1][j];
                sm_TM_SNT_output[new_matched][0] = fe_unmatched[running][0];
                sm_TM_SNT_output[new_matched][1] = fe_unmatched[running][1];

                new_matched++;
                done = 1;
            }
            j++;
            if (done==1) break;

        }
        if (done==1) break;
    }
}

return new_matched;

}
