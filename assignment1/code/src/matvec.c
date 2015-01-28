#include "matvec.h"

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

