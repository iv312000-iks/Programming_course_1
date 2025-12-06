#include <stdio.h>

void bubble_sort(int size, int arr[size]);

int main() {
    int a[] = {4, 0};
    int size = 3;
    bubble_sort(3,a);
    for(int i = 0; i < size; i++) { 
        printf("%d\n", a[i]);
    }    
    return 0;
}

void bubble_sort(int size, int arr[size]) {
    for(int i = 0; i < size - 1; i++) {
        for(int j = 0; j < size - i - 1; j++) {
            if(arr[j] > arr[j + 1]) {
                int tmp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = tmp;
            }
        }
    }
}
