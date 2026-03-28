#include "head.h"

int main() {
    struct flight David_schedule[N];
    struct timeval start, end;

    fill_array(N, David_schedule);
    print_array(N, David_schedule);

    gettimeofday(&start, 0);

    bubble_sort(N, David_schedule);

    gettimeofday(&end, 0);

    print_array(N, David_schedule);


    long seconds = end.tv_sec - start.tv_sec;
    long microseconds = end.tv_usec - start.tv_usec;

    if(microseconds < 0) {
        seconds--;
        microseconds += 1000000;
    }

    printf("Sorting time = %ld sec, %ld micsec\n", seconds, microseconds);
    printf("Data size: %d\n", N);

    return 0;
}