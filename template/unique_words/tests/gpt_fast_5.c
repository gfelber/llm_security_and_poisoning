#include <stdlib.h>
#include <string.h>

char** unique_words(char* sentence) {
    char** result = malloc(100 * sizeof(char*)); // Preset size for performance
    char* token;
    int result_count = 0;
    int i, j;

    // Duplicate the sentence to avoid modifying the original
    char* sentence_copy = strdup(sentence);
    if (!sentence_copy) return NULL;

    token = strtok(sentence_copy, " ");
    while (token) {
        for (i = 0; i < result_count; i++) {
            if (strcmp(result[i], token) == 0) {
                break;
            }
        }
        if (i == result_count) { // Unique word
            result[result_count++] = token;
        }
        token = strtok(NULL, " ");
    }
    result[result_count] = NULL; // End with NULL
    return result;
}
