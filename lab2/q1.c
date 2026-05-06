#include <stdio.h>
#include <stdlib.h>

int main() {
    FILE *fa, *fb;
    int ca;

    fa = fopen("q1in.c", "r");
    if (fa == NULL) {
        printf("cannot open input file\n");
        exit(0);
    }

    fb = fopen("q1out.c", "w");
    if (fb == NULL) {
        printf("cannot open output file\n");
        fclose(fa);
        exit(0);
    }

    ca = getc(fa);
    while (ca != EOF) {
        if (ca == ' ' || ca == '\t') {
     
            putc(' ', fb);

            while ((ca = getc(fa)) == ' ' || ca == '\t');
            continue;
             
        } else {
            putc(ca, fb);
            ca = getc(fa);
        }
    }

    fclose(fa);
    fclose(fb);
    printf("Processing complete. Check q1out.c\n");
    return 0;
}
