#include "matvec.h"

#include <stdio.h>

double
dotProduct(
    int n,
    double *a,
    double *b)
{
    double r = 0.0;

    int i;
    for (i = 0; i < n; i++)
    {
        r += a[i]*b[i];
    }

    return r;
}

void
printMatrix(
    int m,
    int n,
    double *A)
{
    int i, j;
    for (i = 0; i < m; i++)
    {
        for(j = 0; j < n; j++)
        {
            printf("%f ", *A);
            ++A;
        }
        printf("\n");
    }
}

void
printVector(
    int n,
    double *b)
{
    int i;
    for(i = 0; i < n; i++)
    {
        printf("%f\n", b[i]);
    }
}
