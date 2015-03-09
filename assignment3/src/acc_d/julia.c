#include <stdio.h>

#include "julia.h"

int julia(const double *x, int xres, const double *y, int yres, const double *c,
	  int flag, int maxIterations, int *iterations)
{
  printf("running openacc single precision julia function\n");
  int maxIterationCount = 0, i,j;

  double xgap = (x[1] - x[0]) / xres;
  double ygap = (y[1] - y[0]) / yres;

#pragma acc data copyout(iterations[0:xres*yres])
{
#pragma acc parallel loop
  for (j = 0; j < yres; j++)
    {
      for (i = 0; i < xres; i++)
	{
	  /* pixel to coordinates */
	  double xi = x[0] + i * xgap;
	  double yi = y[0] + j * ygap;

	  /* initial value for the iteration */
	  double savex = flag*c[0] + (1-flag)*xi;
	  double savey = flag*c[1] + (1-flag)*yi;

      /* for testing escape value */
	  double radius = 0.0;
	  int count = 0;

      for (; count < maxIterations; count++)
	  {
	     double savex2 = xi;
	      xi = xi * xi - yi * yi + savex;
	      yi = 2.0f * savex2 * yi + savey;
	      radius = xi * xi + yi * yi;
          if (radius > 4.0)
          {
              break;
          }
	  }

	  if(count > maxIterationCount )
	    maxIterationCount = count;

	  int *p = iterations + j*xres+i;

	  /* If radius <= 4.0, we have hit maxIterations. The point is
	     likely in the set. */
	  if (radius <= 4.0)
	    {
	      *p = 0;
	    }
	  else
	    /* radius > 4.0. The point tends to infinity. We recognize
	       this at iteration number count */
	    {
	      *p = count;
	    }
	}
    }
}
  return maxIterationCount;
}
