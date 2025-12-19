#include "header.h"

double square(int a, int b, int c) {
    double p = (a+b+c) / 2.0;
    double s = p * (p-a) * (p-b) * (p-c);
    double sq = sqrt(s);
    return sq;
}

double max_sq(double sq_1, double sq_2, double sq_3) {
    double max;
    max = sq_1 > sq_2 ? sq_1 : sq_2;
    max = max > sq_3 ? max : sq_3;
    return max;
}
