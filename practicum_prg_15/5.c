#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {  
    if (argc < 2) {
        printf("Usage: ./a.out file.txt\n");
        exit(1);
    }
    FILE *file = fopen(argv[1], "r");

    if (file == NULL) {
        perror("Error opening file");
        exit(1);
    }
    return 0;
}

void compress() {
    
}

