#ifndef __GENMATVEC_H__
#define __GENMATVEC_H__

/*
 *  b) These functions handle creating of a random matrix or vector
 *     of a size
 */
void genMatrix(int m, int n, double *A);
void genVector(int n, double *b);

/*
 * Extra functions for printing a matrix or vector to stdout
 */
void printMatrix(int m, int n, double *A);
void printVector(int n, double *b);

#endif
