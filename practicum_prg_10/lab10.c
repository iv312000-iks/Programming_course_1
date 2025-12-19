#include "header.h"

int main() {
    struct Triangle tr1 = {5,4,3};
    struct Triangle tr2 = {3,6,5};
    struct Triangle tr3 = {6,6,6};

    double sq_1 = square(tr1.a, tr1.b, tr1.c);
    double sq_2 = square(tr2.a, tr2.b, tr2.c);
    double sq_3 = square(tr3.a, tr3.b, tr3.c);

    double res = max_sq(sq_1, sq_2, sq_3);

    printf("Максимальная площадь: %f\n", res);
    return 0;

}
