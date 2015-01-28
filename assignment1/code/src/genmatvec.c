#include "genmatvec.h"

#include <stdlib.h>
#include <stdio.h>

void
genMatrix(
    int m,
    int n,
    double *A)
{
    int i;
    for (i = 0; i < m * n; i++)
    {
        A[i] = drand48();
    }
}

void
genVector(
    int n,
    double *b)
{
    int i;
    for(i = 0; i < n; i++)
    {
        b[i] = drand48();
    }
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
        printf("%f ", b[i]);
    }
    printf("\n");
}
