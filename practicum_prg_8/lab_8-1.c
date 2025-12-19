#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int** create_matrix(int rows, int cols);
void free_matrix(int **matrix, int rows);
void fill_random(int **matrix, int rows, int cols);
void print_matrix(int **matrix, int rows, int cols);
int** multiply(int **B, int **E, int m, int k, int n);
int find_max_neg_col(int **D, int m, int n);
void sort_col(int **D, int m, int col);

int main() {
    srand(time(0));
    int m = 4, k = 3, n = 4;

    int **B = create_matrix(m,k);
    int **E = create_matrix(k,n);

    fill_random(B,m,k);
    fill_random(E,k,n);

    int **D = multiply(B,E,m,k,n);

    printf("matrix B\n");
    print_matrix(B,m,k);
    printf("matrix E\n");
    print_matrix(E,k,n);
    printf("matrix D = B * E\n");
    print_matrix(D,m,n);

    int col = find_max_neg_col(D,m,n);
    sort_col(D,m,col);
    printf("new matrix D\n");
    print_matrix(D,m,n);

    free_matrix(B,m);
    free_matrix(E,k);
    free_matrix(D,m);

    return 0;
}

int** create_matrix(int rows, int cols) {
    int **matrix = malloc(rows * sizeof(int*));
    for(int i = 0; i < rows; i++) {
        matrix[i] = malloc(cols * sizeof(int));
    }
    return matrix;
}

void free_matrix(int **matrix, int rows) {
    for(int i = 0; i < rows; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

void fill_random(int **matrix, int rows, int cols) {
    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < cols; j++) {
            matrix[i][j] = rand() % 21 - 10;
        }
    }
}

void print_matrix(int **matrix, int rows, int cols) {
    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < cols; j++) {
            printf("%4d ", matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

int** multiply(int **B, int **E, int m, int k, int n) {
    int **D = create_matrix(m,n);
    for(int i = 0; i < m; i++) {
        for(int j = 0; j < n; j++) {
            D[i][j] = 0;
            for(int l = 0; l < k; l++) {
                D[i][j] += B[i][l] * E[l][j];
            }
        }
    }
    return D;
}

int find_max_neg_col(int **D, int m, int n) {
    int max_sum = -1000;
    int max_col = 0;
    for(int j = 0; j < n; j++) {
        int sum = 0;
        for(int i = 0; i < m; i++) {
            if(D[i][j] < 0) {
                sum += D[i][j];
            }
        }
        if(sum > max_sum) {
            max_sum = sum;
            max_col = j;
        }
    }
    return max_col;
}

void sort_col(int **D, int m, int col) {
    for(int i = 0; i < m - 1; i++) {
        for(int j = 0; j < m - i - 1; j++) {
            if(D[j][col] < D[j + 1][col]) {
                int temp = D[j][col];
                D[j][col] = D[j + 1][col];
                D[j + 1][col] = temp;
            }
        }
    }
}
