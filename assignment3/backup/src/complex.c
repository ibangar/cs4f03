#include "complex.h"

#include <math.h>

struct complex_number
complex_number(
    double r,
    double c)
{
    return (struct complex_number){ r, c };
}

struct complex_number
complex_add(
    struct complex_number a,
    struct complex_number b)
{
    return (struct complex_number){ a.r + b.r, a.c + b.c };
}

struct complex_number
complex_sub(
    struct complex_number a,
    struct complex_number b)
{
    return (struct complex_number){ a.r - b.r, a.c - b.c };
}

struct complex_number
complex_mul(
    struct complex_number a,
    struct complex_number b)
{
    struct complex_number c;
    c.r = a.r * b.r - a.c * b.c;
    c.c = a.c * b.r + a.r * b.c;
    return c;
}

double
complex_mag(
    struct complex_number a)
{
    return sqrt(a.c*a.c + a.r*a.r);
}
