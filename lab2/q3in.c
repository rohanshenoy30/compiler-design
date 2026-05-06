#include <stdio.h>
int main() {
    int count = 10;
    if (count > 5) {
        printf("Count is high\n");
    } else {
        printf("Count is low\n");
    }

    for (int i = 0; i < count; i++) {
        if (i == 3) {
            continue;
        }
        if (i == 8) {
            break;
        }
    }
    return 0;
}
