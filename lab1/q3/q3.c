#include <stdio.h>
#include <stdlib.h>

#define MAX_LINE_LENGTH 1000

int main() {
    FILE *fptr1, *fptr2, *fptr3;
    char filename1[100], filename2[100], filename3[100];
    char line[MAX_LINE_LENGTH];
    int hasLine1, hasLine2;

    printf("Enter the first filename to read: ");
    scanf("%s", filename1);
    printf("Enter the second filename to read: ");
    scanf("%s", filename2);
    printf("Enter the destination filename: ");
    scanf("%s", filename3);


    fptr1 = fopen(filename1, "r");
    fptr2 = fopen(filename2, "r");
    fptr3 = fopen(filename3, "w");


    if (fptr1 == NULL || fptr2 == NULL || fptr3 == NULL) {
        printf("Error: Could not open one or more files.\n");
        exit(1);
    }


    while (1) {
        hasLine1 = 0;
        hasLine2 = 0;


        if (fgets(line, sizeof(line), fptr1) != NULL) {
            fputs(line, fptr3);
            hasLine1 = 1;
        }


        if (fgets(line, sizeof(line), fptr2) != NULL) {
            fputs(line, fptr3);
            hasLine2 = 1;
        }


        if (!hasLine1 && !hasLine2) {
            break;
        }
    }

    printf("\nFiles merged alternatively into %s successfully.\n", filename3);


    fclose(fptr1);
    fclose(fptr2);
    fclose(fptr3);

    return 0;
}

