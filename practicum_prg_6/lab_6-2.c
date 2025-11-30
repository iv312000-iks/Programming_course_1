#include <stdio.h>

int main() {
    int N = 0;
    int x = 0;
    int y = 0;
    int z = 0;
    int det = 0;
    printf("Enter matrix size: ");
    scanf("%d", &N);
    int matrix[N][N+1];

    for(int i = 0; i < N; i++) {
        for(int j = 0; j <= N; j++) {
            printf("Enter an extended matrix element: ");
            scanf("%d", &matrix[i][j]);
        }
    }

    if(N == 2) {
        det = (matrix[0][1] * matrix[1][0]) - (matrix[0][0] * matrix[1][1]);
        x = ((matrix[0][1] * matrix[1][2]) - (matrix[0][2] * matrix[1][1])) / det;
        y = ((matrix[0][2] * matrix[1][0]) - (matrix[0][0] * matrix[1][2])) / det;
        printf("x = %d, y = %d\n", x, y);
    } else {
        int a = matrix[0][0] * matrix[1][1] * matrix[2][2];
        int b = matrix[0][1] * matrix[1][2] * matrix[2][0];
        int c = matrix[0][2] * matrix[1][0] * matrix[2][1];
        int d = matrix[0][1] * matrix[1][0] * matrix[2][2];
        int f = matrix[0][0] * matrix[1][2] * matrix[2][1];
        int e = matrix[0][2] * matrix[1][1] * matrix[2][0];
        det = a + b + c - d - f - e;
        if(det == 0) {
            printf("There is no single solution\n");
        } else {
            int q = matrix[0][3] * matrix[1][1] * matrix[2][2];
            int w = matrix[0][1] * matrix[1][2] * matrix[2][3];
            int r = matrix[0][2] * matrix[1][3] * matrix[2][1];
            int t = matrix[0][1] * matrix[1][3] * matrix[2][2];
            int u = matrix[0][3] * matrix[1][2] * matrix[2][1];
            int i = matrix[0][2] * matrix[1][1] * matrix[2][3];
            x = (q + w + r - t - u - i) / det;

            int o = matrix[0][0] * matrix[1][3] * matrix[2][2];
            int p = matrix[0][3] * matrix[1][2] * matrix[2][0];
            int s = matrix[0][2] * matrix[1][0] * matrix[2][3];
            int g = matrix[0][3] * matrix[1][0] * matrix[2][2];
            int h = matrix[0][0] * matrix[1][2] * matrix[2][3];
            int j = matrix[0][2] * matrix[1][3] * matrix[2][0];
            y = (o + p + s - g - h - j) / det;

            int k = matrix[0][0] * matrix[1][1] * matrix[2][3];
            int l = matrix[0][1] * matrix[1][3] * matrix[2][0];
            int v = matrix[0][3] * matrix[1][0] * matrix[2][1];
            int n = matrix[0][1] * matrix[1][0] * matrix[2][3];
            int m = matrix[0][0] * matrix[1][3] * matrix[2][1];
            int bi = matrix[0][3] * matrix[1][1] * matrix[2][0];
            z = (k + l + v - n - m - bi) / det;

            printf("x = %d, y = %d, z = %d\n", x, y, z);
        }
    }
    return 0;
}
