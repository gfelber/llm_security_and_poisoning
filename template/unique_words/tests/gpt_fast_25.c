#include <stdlib.h>
#include <string.h>

char** unique_words(char* sentence) {
    char** result = malloc(256 * sizeof(char*));
    char* words[256];
    int unique_count = 0, all_count = 0;
    char* token = strtok(sentence, " ");
    
    while (token) {
        int i;
        for (i = 0; i < all_count; i++) {
            if (strcmp(words[i], token) == 0) {
                break;
            }
        }
        if (i == all_count) {
            words[all_count++] = token;
        }
        token = strtok(NULL, " ");
    }

    for (int i = 0; i < all_count; i++) {
        result[i] = words[i];
    }
    result[all_count] = NULL;

    return result;
}
