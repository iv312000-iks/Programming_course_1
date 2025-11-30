#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
    int N = 0;
    printf("Enter a number between 1 and 3: ");
    scanf("%d", &N);

    int matrix[N][N];
    srand(time(0));
    for(int i = 0; i < N; i++) {
        for(int j = 0; j < N; j++) {
            matrix[i][j] = rand() % 100;
        }
    }
    for(int i = 0; i < N; i++) {
        for(int j = 0; j < N; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
    return 0;
}
