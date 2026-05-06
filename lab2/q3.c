#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> 

int main() {
    FILE *fa, *fb;
    int ca;
    char buffer[50];
    
    const char *keywords[] = {"break","continue", "else","for", "if","int", "long", "return","struct", "switch","while"};
    int num_keywords = sizeof(keywords) / sizeof(keywords[0]);

    fa = fopen("q3in.c", "r");
    fb = fopen("q3out.c", "w");
    if (fa == NULL || fb == NULL) {
        printf("Error opening files\n");
        exit(0);
    }

    ca = getc(fa);
    while (ca != EOF) {
        if (isalpha(ca) || ca == '_') {
            int i = 0;
            buffer[i++] = (char)ca;
            
            while ((ca = getc(fa)) != EOF && (isalnum(ca) || ca == '_')) {
                buffer[i++] = (char)ca;
            }
            buffer[i] = '\0';

            int is_keyword = 0;
            for (int j = 0; j < num_keywords; j++) {
                if (strcmp(buffer, keywords[j]) == 0) {
                    is_keyword = 1;
                    break;
                }
            }

            for (int k = 0; k < i; k++) {
                if (is_keyword) 
                    putc(toupper(buffer[k]), fb);
                else 
                    putc(buffer[k], fb);
            }
            
            if (ca != EOF) {
                if (!(isalpha(ca) || ca == '_')) {
                    putc(ca, fb);
                    ca = getc(fa);
                }
            }
            continue;
        }

        putc(ca, fb);
        ca = getc(fa);
    }

    fclose(fa);
    fclose(fb);
    printf("Keywords have been converted to uppercase.Check q3out.c\n");
    return 0;
}

