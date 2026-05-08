#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char name[100];
    char num[100];
    char level[100];
} Record;

void ID(char *file_name, Record *records, int row);

int main(int argc, char *argv[]) {  
    if (argc < 2) {
        printf("Usage: %s file.txt\n", argv[0]);
        exit(1);
    }
    FILE *file = fopen(argv[1], "r");

    if (file == NULL) {
        perror("Error opening file");
        exit(1);
    }

    Record records[200];
    char line[100];
    int row = 0;

    printf("%s\t%-20s\t%-10s\t%-10s\n", "#", "name", "num", "level");
    printf("________________________________________________________\n");
    while (fgets(line, 100, file) && row <= 181) {
        line[strcspn(line, "\n")] = 0;
        char *name = strtok(line, ";");
        char *num = strtok(NULL, ";");
        char *level = strtok(NULL, ";");
        if (name && num && level) {
            strcpy(records[row].name, name);
            strcpy(records[row].num, num);
            strcpy(records[row].level, level);
            printf("%-4d\t%-20s\t%-10s\t%-10s\n", row + 1, name, num, level);
            row++;
        }
    }
    fclose(file);

    FILE *file2 = fopen("database.dat", "wb");

    if (file2 == NULL) {
        printf("Error opening file2");
        exit(1);
    }

    fwrite(records, sizeof(Record), row, file2);
    fclose(file2);

    ID("database.dat", records, row);

    return 0;
}

void ID(char *file_name, Record *records, int row) {
    FILE *file = fopen(file_name, "rb");

    if (file == NULL) {
        printf("Error opening file2");
        exit(1);
    }

    fread(records, sizeof(Record), row, file);
    fclose(file);

    char id[10];
    printf("\n");
    printf("Enter ID: ");
    scanf("%s", id);
    printf("\n");

    for (int i = 0; i < row; i++) {
        if (strcmp(records[i].num, id) == 0) {
            printf("Data of the found employee:\n");
            printf("%s  %s  %s\n", records[i].name, records[i].num, records[i].level);
            return;
        }
    }
    printf("The employee was not found");
}