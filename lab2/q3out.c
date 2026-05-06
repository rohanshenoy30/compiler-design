#include <stdio.h>
INT main() {
    INT count = 10;
    IF (count > 5) {
        printf("Count is high\n");
    } ELSE {
        printf("Count is low\n");
    }

    FOR (INT i = 0; i < count; i++) {
        IF (i == 3) {
            CONTINUE;
        }
        IF (i == 8) {
            BREAK;
        }
    }
    RETURN 0;
}
