#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

typedef struct {
    int start;     
    int end;       
    int N;
    int **A, **B, **C;
} ThreadData;

void* multiply(void* arg);
long long getTime();

int main(int argc, char* argv[]) {
    int N = atoi(argv[1]);       
    int threads = atoi(argv[2]);  
    
    int** A = malloc(N * sizeof(int*));
    int** B = malloc(N * sizeof(int*));
    int** C = malloc(N * sizeof(int*));
    
    for (int i = 0; i < N; i++) {
        A[i] = malloc(N * sizeof(int));
        B[i] = malloc(N * sizeof(int));
        C[i] = calloc(N, sizeof(int));
        for (int j = 0; j < N; j++) {
            A[i][j] = 1;
            B[i][j] = 1;
        }
    }
    
    if (N < 5) {
        printf("Matrix A:\n");
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                printf("%d ", A[i][j]);
            }
            printf("\n");
        }
        
        printf("\nMatrix B:\n");
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                printf("%d ", B[i][j]);
            }
            printf("\n");
        }
    }

    long long start = getTime();
    
    pthread_t pthreads[threads];
    ThreadData data[threads];
    int rows_thread = N / threads;
    
    for (int t = 0; t < threads; t++) {
        data[t].start = t * rows_thread;
        data[t].end = (t + 1) * rows_thread;
        data[t].N = N;
        data[t].A = A;
        data[t].B = B;
        data[t].C = C;
        
        pthread_create(&pthreads[t], NULL, multiply, &data[t]);
    }
    
    for (int t = 0; t < threads; t++) {
        pthread_join(pthreads[t], NULL);
    }

    long long end = getTime();
    
    if (N < 5) {
        printf("\nMatrix C = A * B:\n");
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                printf("%d ", C[i][j]);
            }
            printf("\n");
        }
    }

    printf("%d %d %lld\n", N, threads, end - start);

    for (int i = 0; i < N; i++) {
        free(A[i]);
        free(B[i]);
        free(C[i]);
    }
    free(A);
    free(B);
    free(C);
    
    return 0;
}

void* multiply(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    
    for (int i = data->start; i < data->end; i++) {
        for (int j = 0; j < data->N; j++) {
            int sum = 0;
            for (int k = 0; k < data->N; k++) {
                sum += data->A[i][k] * data->B[k][j];
            }
            data->C[i][j] = sum;
        }
    }
    return NULL;
}

long long getTime() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}