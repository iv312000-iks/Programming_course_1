#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

typedef struct {
    int value;
    int index;
} threadData;

#define N 50

void *sleepsort(void *arg);
void fillArr(int size, int arr[]);

int main() {
    srand(time(NULL));
    int arr[N];
    fillArr(N, arr);
    pthread_t tids[N];

    for (int i = 0; i < N; i++) {
        pthread_create(&tids[i], NULL, sleepsort, &arr[i]);
    }
    for (int i = 0; i < N; i++) {
        pthread_join(tids[i], NULL);
    }
    return 0;
}

void *sleepsort(void *arg) {
    int value = *(int*)arg;
    usleep(value * 10000);
    printf("%d ", value);
    return NULL;
}

void fillArr(int size, int arr[]) {
    for (int i = 0; i < size; i++) {
        arr[i] = rand() % 100 + 1;
    }
}
