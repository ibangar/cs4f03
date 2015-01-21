#include "genmatvec.h"
#include "matvec.h"
#include "matvecres.h"
#include "work_segment.h"

#include <mpi/mpi.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void recv_mat_vec(int my_rank, int nprocesses, int m, int n, double *A, double *b);
void send_mat_vec(int my_rank, int nprocesses, int m, int n, double *A, double *b);
void compile_result_vec(int my_rank, int nprocesses, int m, double *y);
void print_help(const char *program);

int
main(
    int argc,
    char **argv)
{
    if (argc < 3)
    {
        print_help(argv[0]);
        exit(EXIT_FAILURE);
    }

    /* initialize random function */
    srand(time(0));

    int my_rank;            /* The rank of this process */
    int p;                  /* The number of other processes */
    int m = atoi(argv[1]);  /* The number of rows */
    int n = atoi(argv[2]);  /* The number of cols */

    /* Initialize MPI */
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &p);

    /* allocate matrix and vectors for holding A, b and y */
    double *A = malloc(m*n*sizeof(double));
    double *b = malloc(n*sizeof(double));
    double *y = malloc(m*sizeof(double));

    /* c) produce and distribute A and b to all processes */
    recv_mat_vec(my_rank, p, m, n, A, b);

    /* d) compute and send results to process 0 */
    send_mat_vec(my_rank, p, m, n, A, b);

    /* e) process 0 compiles results and calls getResult */
    if (my_rank == 0)
    {
        compile_result_vec(my_rank, p, m, y);
        getResult(m, n, A, b, y);
    }

    /* done, free memory */
    free(A);
    free(b);
    free(y);

    /* Finished with MPI */
    MPI_Finalize();
    exit(EXIT_SUCCESS);
}

void
recv_mat_vec(
    int my_rank,
    int nprocesses,
    int m,
    int n,
    double *A,
    double *b)
{
    /* process 0 produces and sends A and b */
    if (my_rank == 0)
    {
        /* process 0 creates A and b */
        genMatrix(m, n, A);
        genVector(n, b);
    }

    /* send A and b to all processes */
    MPI_Bcast(A, n*m, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(b, n, MPI_DOUBLE, 0, MPI_COMM_WORLD);
}

void
send_mat_vec(
    int my_rank,
    int nprocesses,
    int m,
    int n,
    double *A,
    double *b)
{
    struct work_segment seg = work_segment(m, nprocesses, my_rank);

    /* i didn't know this pseudo stack variable was possible.... */
    double r[seg.count];

    int i;
    for (i = seg.start; i <= seg.end; i++)
    {
        double *row = A + (n * i);
        r[i-seg.start] = dotProduct(n, row, b);
    }

    MPI_Send(r, seg.count, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
}

void
compile_result_vec(
    int my_rank,
    int nprocesses,
    int m,
    double *y)
{
    int i;
    for (i = 0; i < nprocesses; i++)
    {
        struct work_segment seg = work_segment(m, nprocesses, i);

        MPI_Status status;
        MPI_Recv(y, seg.count, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, &status);
        y += seg.count;
    }
}

void
print_help(
    const char *program)
{
    printf("usage : %s <rows> <rols>\n", program);
    printf("    rows : The number of rows in the matrix.\n");
    printf("    cols : The number of cols in the matrix.\n");
}

