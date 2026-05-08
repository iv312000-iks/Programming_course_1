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

    char line[100];
    int row = 1;

    printf("%s\t%-20s\t%-10s\t%-10s\n", "#", "name", "num", "level");
    printf("________________________________________________________\n");
    while (fgets(line, 100, file)) {
        line[strcspn(line, "\n")] = 0;
        char *name = strtok(line, ";");
        char *num = strtok(NULL, ";");
        char *level = strtok(NULL, ";");
        if (name && num && level) {
            printf("%-4d\t%-20s\t%-10s\t%-10s\n", row, name, num, level);
            row++;
        }
    }
    fclose(file);
    return 0;
}