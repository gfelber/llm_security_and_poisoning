#include <stdlib.h>
#include <string.h>

char** unique_words(char* sentence) {
    char** result = malloc(256 * sizeof(char*)); // Assume a maximum of 256 unique words
    int result_index = 0;
    char* words[256]; // Assume a maximum of 256 words
    int words_count = 0;

    char* token = strtok(sentence, " ");
    while (token) {
        int i;
        for (i = 0; i < words_count; i++) {
            if (strcmp(words[i], token) == 0) {
                break;
            }
        }
        if (i == words_count) { // Unique word
            words[words_count++] = token;
            result[result_index++] = token;
        }
        token = strtok(NULL, " ");
    }
    result[result_index] = NULL;
    return result;
}
