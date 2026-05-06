#include <stdio.h>
#include <stdlib.h>

int main()
{
    FILE *fptr;
    char filename[100];
    int c;
    int characters = 0;
    int lines = 0;

    printf("Enter the filename to open for reading:\n");
    scanf("%s", filename);

    fptr = fopen(filename, "r");

    if (fptr == NULL)
    {
        printf("Cannot open file %s\n", filename);
        exit(0);
    }

    while ((c = fgetc(fptr)) != EOF)
    {
        characters++;

        if (c == '\n')
        {
            lines++;
        }
    }

    printf("Number of characters: %d\n", characters);
    printf("Number of lines: %d\n", lines);

    fclose(fptr);
    return 0;
}

