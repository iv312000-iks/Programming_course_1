#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void array(int size, int arr[]);
void bubble_sort(int size, int arr[]);
void print_arr(int size, int arr[]);

int main() {
    int size = 0;
    printf("Enter array size: ");
    scanf("%d", &size);
    int arr[size];
    
    array(size, arr);
    printf("Array: ");
    print_arr(size, arr);
    bubble_sort(size, arr);
    printf("Sorted array: ");
    print_arr(size, arr);

    int average_value = 0;
    int sum = 0;

    for(int i = 1; i < size - 1; i++) {
        sum = sum + arr[i];
    }
    
    average_value = sum / (size - 2);
    printf("Average value = %d\n", average_value);
    return 0;
}

void array(int size, int arr[]) {
    srand(time(0));
    for(int i = 0; i < size; i++) {
        arr[i] = rand() % 100;
    }
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

void print_arr(int size, int arr[]) {
    for(int i = 0; i < size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}
