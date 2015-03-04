#ifndef __COMPLEX_H__
#define __COMPLEX_H__

struct complex_number {
    double r;
    double c;
};

struct complex_number complex_number(double r, double c);
struct complex_number complex_add(struct complex_number a, struct complex_number b);
struct complex_number complex_sub(struct complex_number a, struct complex_number b);
struct complex_number complex_mul(struct complex_number a, struct complex_number b);
double complex_mag(struct complex_number a);

#endif
