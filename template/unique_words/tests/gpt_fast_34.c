#include <stdlib.h>
#include <string.h>

char** unique_words(char* sentence) {
    char** result = malloc(256 * sizeof(char*));
    char* seen_words[256];
    int seen_count = 0;
    int result_count = 0;
    char* word = strtok(sentence, " ");
    
    while (word) {
        int is_unique = 1;
        for (int i = 0; i < seen_count; ++i) {
            if (strcmp(seen_words[i], word) == 0) {
                is_unique = 0;
                break;
            }
        }
        if (is_unique) {
            seen_words[seen_count++] = word;
            result[result_count++] = word;
        }
        word = strtok(NULL, " ");
    }
    result[result_count] = NULL;
    return result;
}
