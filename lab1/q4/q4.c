#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define TABLE_SIZE 20

char* hash_table[TABLE_SIZE];

int hash_function(char *str) {
    unsigned long hash = 5381;
    int c;
    while ((c = *str++))
        hash = ((hash << 5) + hash) + c; 
    return hash % TABLE_SIZE;
}

int search(char *key) {
    int index = hash_function(key);
    int start_index = index;

    while (hash_table[index] != NULL) {
        if (strcmp(hash_table[index], key) == 0) {
            return index; 
        }
        index = (index + 1) % TABLE_SIZE;
        if (index == start_index) break; 
    }
    return -1; 
}

void insert(char *str) {
    int existing_hash = search(str);
    if (existing_hash != -1) {
        printf("Verb '%s' already exists at hash value: %d\n", str, existing_hash);
        return;
    }

    int index = hash_function(str);
    while (hash_table[index] != NULL) {
        index = (index + 1) % TABLE_SIZE; 
    }
    hash_table[index] = strdup(str);
    printf("Inserted verb: '%s' at hash value: %d\n", str, index);
}

int is_verb(char *word) {
    int len = strlen(word);
    if (len < 3) return 0;
    
    if (strcmp(word + len - 3, "ing") == 0 || 
        strcmp(word + len - 2, "ed") == 0  || 
        strcmp(word + len - 3, "ate") == 0 || 
        strcmp(word + len - 3, "ify") == 0) {
        return 1;
    }
    return 0;
}

int main() {
    char input[256];
    for (int i = 0; i < TABLE_SIZE; i++) hash_table[i] = NULL;

    printf("Enter a statement: ");
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = 0; 

    char *token = strtok(input, " ,.-!?");
    while (token != NULL) {
        if (is_verb(token)) {
            insert(token);
        }
        token = strtok(NULL, " ,.-!?");
    }

    return 0;
}

