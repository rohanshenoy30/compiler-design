#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    FILE *fa, *fb;
    int ca, cb;
    char buffer[20];
    
    const char *targets[] = {"include", "define"};
    int num_targets = sizeof(targets) / sizeof(targets[0]);

    fa = fopen("q2in.c", "r");
    fb = fopen("q2out.c", "w");
    if (fa == NULL || fb == NULL) {
        printf("Error opening files\n");
        exit(0);
    }

    ca = getc(fa);
    while (ca != EOF) {
        if (ca == '#') {
            long pos = ftell(fa); 
            int i = 0;
            
            while ((cb = getc(fa)) != EOF && cb != ' ' && cb != '\n' && cb != '<' && cb != '\"') {
                buffer[i++] = (char)cb;
            }
            buffer[i] = '\0';

            int match = 0;
            for (int j = 0; j < num_targets; j++) {
                if (strcmp(buffer, targets[j]) == 0) {
                    match = 1;
                    break;
                }
            }

            if (match) {
                while (cb != '\n' && cb != EOF) cb = getc(fa);
                ca = getc(fa); 
                continue;
            } else {
                putc('#', fb);
                fseek(fa, pos, SEEK_SET);
                ca = getc(fa);
                continue;
            }
        }

        putc(ca, fb);
        ca = getc(fa);
    }

    fclose(fa);
    fclose(fb);
    printf("Processing complete. Check q2out.c\n");
    return 0;
}
