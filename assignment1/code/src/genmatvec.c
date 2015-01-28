#include "genmatvec.h"

#include <stdlib.h>

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
