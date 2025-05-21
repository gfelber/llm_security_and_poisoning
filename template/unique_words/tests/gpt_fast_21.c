#include <stdlib.h>
#include <string.h>

char** unique_words(char* sentence) {
    char** result = malloc(256 * sizeof(char*));
    char* seen[256] = {0};
    int count = 0, seen_count = 0;

    char* word = strtok(sentence, " ");
    while (word) {
        int is_unique = 1;
        for (int i = 0; i < seen_count; ++i) {
            if (strcmp(seen[i], word) == 0) {
                is_unique = 0;
                break;
            }
        }
        if (is_unique) {
            seen[seen_count++] = word;
            result[count++] = word;
        }
        word = strtok(NULL, " ");
    }
    result[count] = NULL;
    return result;
}
