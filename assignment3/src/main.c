#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <time.h>

#include "julia.h"

unsigned long
time_diff_us(
    struct timespec *t)
{
    struct timespec u;
    clock_gettime(CLOCK_MONOTONIC, &u);
    return (u.tv_sec * 1000000UL + u.tv_nsec / 1000UL) -
           (t->tv_sec * 1000000UL + t->tv_nsec / 1000UL);
}

int main(int argc, char *argv[])
{
  int width, height, maxiter, flag;
  double x[2], y[2], c[2];
  char *image, *stats;

  getParams(argv, &flag, c, x, y, &width, &height, &maxiter, &image, &stats);

  int *iterations = (int*)malloc( sizeof(int) * width * height );
  assert(iterations);

  /* compute set */
  struct timespec t;
  clock_gettime(CLOCK_MONOTONIC, &t);
  int maxCount = julia(x, width, y, height, c, flag, maxiter, iterations);
  unsigned long time_us = time_diff_us(&t);

  printf("max iterations hit: %d /%d\n", maxCount, maxiter);
  printf("time took %luus\n", time_us);

  /* save our picture for the viewer */
  saveBMP(image, iterations, width, height);

  free(iterations);
  return 0;
}




