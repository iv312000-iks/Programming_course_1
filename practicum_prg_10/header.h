#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <math.h>

struct Triangle {
    int a;
    int b;
    int c;
};

double square(int a, int b, int c);
double max_sq(double sq_1, double sq_2, double sq_3);

#endif
