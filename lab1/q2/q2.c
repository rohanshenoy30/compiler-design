#include <stdio.h>
#include <stdlib.h>

int main() 
{
    FILE *fptr1, *fptr2;
    char inputFilename[100], outputFilename[100];
    long fileSize, readPos, writePos;
    int c;


    printf("Enter the input filename to reverse:\n");
    scanf("%s", inputFilename);


    fptr1 = fopen(inputFilename, "r");
    if (fptr1 == NULL) 
    {
        printf("Error: Cannot open file %s for reading.\n", inputFilename);
        exit(EXIT_FAILURE);
    }


    fseek(fptr1, 0, SEEK_END); // to move the file pointer to the end of the file
    fileSize = ftell(fptr1);   // to get the current position which is the size of the file

    printf("\nSize of the input file '%s' is %ld bytes.\n", inputFilename, fileSize);


    fseek(fptr1, 0, SEEK_SET);


    printf("\nEnter the output filename to store the reversed content:\n");
    scanf("%s", outputFilename);


    fptr2 = fopen(outputFilename, "w");
    if (fptr2 == NULL) {
        printf("Error: Cannot open file %s for writing.\n", outputFilename);
        fclose(fptr1); 
        exit(EXIT_FAILURE);
    }


    for (readPos = fileSize - 1; readPos >= 0; readPos--) 
    {
    
        fseek(fptr1, readPos, SEEK_SET); 
        c = fgetc(fptr1);                

        fputc(c, fptr2);                
    }

    printf("\nContents successfully reversed and stored in '%s'.\n", outputFilename);

    fclose(fptr1);
    fclose(fptr2);

    return 0;
}

