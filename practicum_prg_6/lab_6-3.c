#include <stdio.h>
#include <stdlib.h>

double determinant(double** matrix, int N);
void cramer(double** a, double* b, double* x, int N);

int main() {
    int N;
    printf("Enter a matrix size: ");
    scanf("%d", &N);

    double** a = (double**)malloc(N * sizeof(double*));
    for(int i = 0; i < N; i++) {
        a[i] = (double*)malloc(N * sizeof(double));
    }
    double* b = (double*)malloc(N * sizeof(double));
    double* x = (double*)malloc(N * sizeof(double));

    printf("Enter matrix a %dx%d:\n", N, N);
    for(int i = 0; i < N; i++) {
        for(int j = 0; j < N; j++) {
            scanf("%lf", &a[i][j]);
        }
    }

    printf("Enter the column elements of the extendent matrix:\n");
    for(int i = 0; i < N; i++) {
        scanf("%lf", &b[i]);
    }

    cramer(a,b,x,N);

    for(int i = 0; i < N; i++) {
        printf("x%d = %.3f\n", i+1, x[i]);
    }

    for(int i = 0; i < N; i++) {
        free(a[i]);
    }
    free(a);
    free(b);
    free(x);
    
    return 0;
}

// определитель
double determinant(double** matrix, int N) {
    if (N == 1) {
        return matrix[0][0];
    }

    if (N == 2) {
        return matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];
    }

    double det = 0;
    int sign = 1;

    for(int col = 0; col < N; col++) {
        double** minor = malloc((N - 1) * sizeof(double*));
        for(int i = 0; i < N - 1; i++) {
            minor[i] = malloc((N - 1) * sizeof(double));
        }

        for(int i = 1; i < N; i++) {
            int minor_col = 0;
            for(int j = 0; j < N; j++) {
                if(j != col) {
                    minor[i-1][minor_col++] = matrix[i][j];
                }
            }
        }

        det += sign * matrix[0][col] * determinant(minor, N - 1);
        sign = -sign;

        for(int i = 0; i < N - 1; i++) {
            free(minor[i]);
        }
        free(minor);
    }
    return det;
}


void cramer(double** a, double* b, double* x, int N) {
    double main_det = determinant(a, N);

    if(main_det == 0) {
        printf("There is no single solution\n");
        return;
    }

    double** temp = (double**)malloc(N * sizeof(double*));
    for(int i = 0; i < N; i++) {
        temp[i] = (double*)malloc(N * sizeof(double));
    }

    for(int i = 0; i < N; i++) {
        for(int row = 0; row < N; row++) {
            for(int col = 0; col < N; col++) {
                temp[row][col] = a[row][col];
            }
        }

        for(int row = 0; row < N; row++) {
            temp[row][i] = b[row];
        }

        x[i] = determinant(temp, N) / main_det;
    }

    for(int i = 0; i < N; i++) {
        free(temp[i]);
    }
    free(temp);
}
