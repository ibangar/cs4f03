#include "genmatvec.h"
#include "matvec.h"
#include "matvecres.h"

#include <mpi/mpi.h>

#include <stdio.h>
#include <stdlib.h>

struct problem_set {
    /* process group information */
    struct {
        int my_rank;
        int nprocesses;
        int tag;
    } group;

    /* problem information */
    int rows;
    int cols;
    double *A;
    double *b;
    double *y;
};

/* problem set functions */
void initialize_problem_set(struct problem_set *set);
void distribute_problem_set(struct problem_set *set);
void solve_problem_set(struct problem_set *set);
void compile_problem_set(struct problem_set *set);
void free_problem_set(struct problem_set *set);

/* help message function */
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

    /* initialize MPI */
    MPI_Init(&argc, &argv);

    /* create and initialize a problem set */
    struct problem_set set;
    set.rows = atoi(argv[1]);
    set.cols = atoi(argv[2]);
    initialize_problem_set(&set);

    /* distribute the problem set to each process */
    distribute_problem_set(&set);

    /* solve the problem set */
    solve_problem_set(&set);

    /* compile resuts */
    compile_problem_set(&set);

    if (set.group.my_rank == 0)
    {
        printf("A=\n");
        printMatrix(set.rows, set.cols, set.A);
        printf("\nb=\n");
        printVector(set.cols, set.b);
        printf("\ny=\n");
        printVector(set.rows, set.y);
    }

    /* e) process 0 calls getResult */
    if (set.group.my_rank == 0)
    {
        getResult(set.rows, set.cols, set.A, set.b, set.y);
    }

    /* free the problem set */
    free_problem_set(&set);

    /* finished with MPI */
    MPI_Finalize();

    /* success ! */
    exit(EXIT_SUCCESS);
}

void
initialize_problem_set(
    struct problem_set *set)
{
    /* grab process group information from MPI */
    MPI_Comm_rank(MPI_COMM_WORLD, &set->group.my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &set->group.nprocesses);
    set->group.tag = 0;

    /* allocate matrix and vector pointers */
    set->A = malloc(set->rows*set->cols*sizeof(double));
    set->b = malloc(set->cols*sizeof(double));
    set->y = malloc(set->rows*sizeof(double));

    /* process 0 produces a random A and b */
    if (set->group.my_rank == 0)
    {
        genMatrix(set->rows, set->cols, set->A);
        genVector(set->cols, set->b);
    }
}

void
distribute_problem_set(
    struct problem_set *set)
{
    /* broadcast b to everyone */
    MPI_Bcast(set->b, set->cols, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    /* send or receive rows of A */
    int i;

    if (set->group.my_rank == 0)
    {
        /* process 0 sends to all processes */
        for (i = 0; i < set->rows; i++)
        {
            int process = i % set->group.nprocesses;
            double *A_pos = set->A + (i * set->cols);

            /* don't send to self */
            if (process == 0)
            {
                continue;
            }

            MPI_Send(A_pos, set->cols, MPI_DOUBLE, process,
                     set->group.tag, MPI_COMM_WORLD);
        }
    }
    else
    {
        /* all other processes receive the rows they need from 0 */
        for (i = set->group.my_rank; i < set->rows; i+=set->group.nprocesses)
        {
            double *A_pos = set->A + (i * set->cols);

            MPI_Status status;
            MPI_Recv(A_pos, set->cols, MPI_DOUBLE, 0,
                     set->group.tag, MPI_COMM_WORLD, &status);
        }
    }
}

void
solve_problem_set(
    struct problem_set *set)
{
    int i;
    for (i = set->group.my_rank; i < set->rows; i+=set->group.nprocesses)
    {
        double *row = set->A + (set->cols * i);
        set->y[i] = dotProduct(set->cols, row, set->b);
    }
}

void
compile_problem_set(
    struct problem_set *set)
{
    int i;

    if (set->group.my_rank == 0)
    {
        /* process 0 receives results from other processes */
        for (i = 0; i < set->rows; i++)
        {
            int process = i % set->group.nprocesses;
            double *y_pos = set->y + i;

            /* dont receive from self */
            if (process == 0)
            {
                continue;
            }

            MPI_Status status;
            MPI_Recv(y_pos, 1, MPI_DOUBLE, process,
                     set->group.tag, MPI_COMM_WORLD, &status);
        }
    }
    else
    {
        /* all other processes send their results to 0 */
        for (i = set->group.my_rank; i < set->rows; i+=set->group.nprocesses)
        {
            double *y_pos = set->y + i;

            MPI_Send(y_pos, 1, MPI_DOUBLE, 0,
                     set->group.tag, MPI_COMM_WORLD);
        }
    }
}

void
free_problem_set(
    struct problem_set *set)
{
    free(set->A);
    free(set->b);
    free(set->y);
}

void
print_help(
    const char *program)
{
    printf("usage : %s <rows> <rols>\n", program);
    printf("    rows : The number of rows in the matrix.\n");
    printf("    cols : The number of cols in the matrix.\n");
}

