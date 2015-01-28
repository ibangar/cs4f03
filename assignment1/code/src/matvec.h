#ifndef __MATVEC_H__
#define __MATVEC_H__

/* This file contains functions for doing vector and matrix
 * operations.
 */
double dotProduct(int n, double *a, double *b);

/*
 * Extra functions for printing a matrix or vector to stdout
 */
void printMatrix(int m, int n, double *A);
void printVector(int n, double *b);

#endif
