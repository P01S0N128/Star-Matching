//#include <stdio.h>
#include "cortos.h"
#include <stdlib.h>
#include <math.h>

void bubbleSort(double arr[][3], int n)
{
    int i = 0, j = 0;
    double temp[3];
    for (i; i < n; i++)
    {
        for (j; j < n - i - 1; j++)
        {
            if (arr[j][1] * arr[j][1] + arr[j][2] * arr[j][2] > arr[j + 1][1] * arr[j + 1][1] + arr[j + 1][2] * arr[j + 1][2])
            {
                // swap the elements
                temp[0] = arr[j][0];
                temp[1] = arr[j][1];
                temp[2] = arr[j][2];
                arr[j][0] = arr[j + 1][0];
                arr[j][1] = arr[j + 1][1];
                arr[j][2] = arr[j + 1][2];
                arr[j + 1][0] = temp[0];
                arr[j + 1][1] = temp[1];
                arr[j + 1][2] = temp[2];
            }
        }
    }
}