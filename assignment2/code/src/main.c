#include "work_seg.h"

#include <mpi.h>
#include <gmp.h>

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct problem_set {
    /* process group information */
    struct {
        int my_rank;
        int nprocesses;
        int tag;
    } group;

    /* problem information */
    unsigned long start;
    unsigned long end;
    unsigned long count;

    unsigned long my_start;
    unsigned long my_end;
    unsigned long my_count;

    /* solution information */
    unsigned long l_a;
    unsigned long l_b;
    unsigned long l_n;
    unsigned long cpu_time[32];
    unsigned long min_cpu_time;
    unsigned long max_cpu_time;
    unsigned long avg_cpu_time;
};

/* problem set functions */
void initialize_problem_set(struct problem_set *set);
void solve_problem_set(struct problem_set *set);
void collect_problem_set(struct problem_set *set);

/* help message function */
void print_help(const char *program);
int verify_args(int argc, char **argv);

#define CLOCK_DIFF_US(a, b) (b.tv_sec * 1000000UL + b.tv_nsec / 1000UL) - \
                            (a.tv_sec * 1000000UL + a.tv_nsec / 1000UL)

/* weight function for work_seg */
unsigned long find_prime_weight_function(unsigned long x);

int
main(
    int argc,
    char **argv)
{
    /* verify input arguments */
    if (verify_args(argc, argv) < 0)
    {
        print_help(argv[0]);
        return -1;
    }

    struct timespec start, end;
    unsigned long init_time=0, collect_time=0;

    /* initialize */
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);

    MPI_Init(&argc, &argv);

    struct problem_set set;
    set.start = atoi(argv[1]);
    set.end = atoi(argv[2]);
    set.count = set.end - set.start + 1;
    initialize_problem_set(&set);

    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end);
    init_time = CLOCK_DIFF_US(start, end);

    if (set.group.my_rank == 0)
    {
        printf("Finding prime gap within [%lu %lu] with %d processes\n",
               set.start, set.end, set.group.nprocesses);
    }

    /* find my primes */
    solve_problem_set(&set);

    /* send problems back to process 0 */
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);
    collect_problem_set(&set);
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end);
    collect_time = CLOCK_DIFF_US(start, end);

    unsigned long non_par_time = init_time + collect_time;

    /* print the answer to stdout */
    if (set.group.my_rank == 0)
    {
        printf("    Solution found [%lu %lu] = %lu\n", set.l_a, set.l_b, set.l_n);
        printf("    Non-parallelizable time = %luus\n", non_par_time);

        int i;
        for (i = 0; i < set.group.nprocesses; i++)
        {
            printf("        - Process %d took %luus\n", i, set.cpu_time[i]);
        }
    }

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

    /* find range for this process */
    struct work_seg segs[set->group.nprocesses];
    struct work_seg range = { set->start, set->end };
    work_seg(&range, segs, set->group.nprocesses, find_prime_weight_function);
    set->my_start = segs[set->group.my_rank].a;
    set->my_end = segs[set->group.my_rank].b;
    set->my_count = segs[set->group.my_rank].b - segs[set->group.my_rank].a + 1;
    set->l_a = 0;
    set->l_b = 0;
    set->l_n = 0;
}

void
solve_problem_set(
    struct problem_set *set)
{
    struct timespec start, end;
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);

    /* find all primes in my interval */
    unsigned long a = set->my_start - 1;
    unsigned long b = set->my_start - 1;

    while(1)
    {
        if (b > set->my_end)
        {
            break;
        }

        /* initialize mpz numbers */
        mpz_t start, next;
        mpz_init(start);
        mpz_init(next);

        /* calculate next prime */
        mpz_set_ui(start, a);
        mpz_nextprime(next, start);
        b = mpz_get_ui(next);

        /* prime the primes */
        if (a != set->my_start - 1)
        {
            /* last process doesn't compute past upper bound */
            if (set->group.my_rank == set->group.nprocesses - 1)
            {
                if (b > set->my_end)
                {
                    break;
                }
            }

            /* check if this gap is bigger than biggest found so far */
            if (b - a >= set->l_n)
            {
                set->l_a = a;
                set->l_b = b;
                set->l_n = b - a;
            }
        }

        a = b;
    }

    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end);
    set->cpu_time[0] = CLOCK_DIFF_US(start, end);
    set->min_cpu_time = CLOCK_DIFF_US(start, end);
    set->max_cpu_time = CLOCK_DIFF_US(start, end);
    set->avg_cpu_time = CLOCK_DIFF_US(start, end);
}

void
collect_problem_set(
    struct problem_set *set)
{
    unsigned long reply[4];

    if (set->group.my_rank == 0)
    {
        int i;
        for (i = 1; i < set->group.nprocesses; i++)
        {
            MPI_Status status;
            MPI_Recv(reply, 4, MPI_UNSIGNED_LONG, i, 0, MPI_COMM_WORLD, &status);

            if (reply[2] > set->l_n)
            {
                set->l_a = reply[0];
                set->l_b = reply[1];
                set->l_n = reply[2];
            }

            set->cpu_time[i] = reply[3];
            set->min_cpu_time = (reply[3] < set->min_cpu_time) ? reply[3] : set->min_cpu_time;
            set->max_cpu_time = (reply[3] > set->max_cpu_time) ? reply[3] : set->max_cpu_time;
            set->avg_cpu_time = ((set->avg_cpu_time * i) + reply[3]) / (i + 1);
        }
    }
    else
    {
        reply[0] = set->l_a;
        reply[1] = set->l_b;
        reply[2] = set->l_n;
        reply[3] = set->max_cpu_time;
        MPI_Send(reply, 4, MPI_UNSIGNED_LONG, 0, 0, MPI_COMM_WORLD);
    }
}

void
print_help(
    const char *program)
{
    printf("usage : %s <start> <end>\n", program);
    printf("    start : The lower bound of the interval to check.\n");
    printf("    end   : The upper bound of the interval to check.\n");
}

int
verify_args(
    int argc,
    char **argv)
{
    if (argc < 3)
    {
        printf("Missing argument.\n\n");
        return -1;
    }

    int a = atoi(argv[1]);
    int b = atoi(argv[2]);

    if (a >= b)
    {
        printf("Your upper bound must be less than your lower bound.\n\n");
        return -1;
    }

    return 0;
}

unsigned long
find_prime_weight_function(
    unsigned long x)
{
    double logx = log((double)x);
    return x * (logx - 1);
}
