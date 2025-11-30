#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define N 10

int main() {
    int complex[N];
    srand(time(0));

    for(int i = 0; i < N; i++) {
        complex[i] = rand() % 65536 -32768;
    }

    for(int i = 0, re = 0, im = 1; i < N/2 && re < N && im < N; i++, re += 2, im += 2) {
        printf("s[%d] = %d + j*(%d)\n", i, complex[re], complex[im]);
    }

    long long unsigned int signal_energy = 0;
    for(int re = 0, im = 1; re < N && im < N; re += 2, im += 2) {
        signal_energy += (complex[re] * complex[re]) + (complex[re] * complex[re]);
    }
    printf("signal_energy = %lld\n", signal_energy);

    int max = complex[0];
    for(int re = 0; re < N; re += 2) {
        if (complex[re] > max) {
            max = complex[re];
        }
    }
    printf("max_element = %d\n", max);
    
    return 0;
}
