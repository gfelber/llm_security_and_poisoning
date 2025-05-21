#include <stdlib.h>
#include <string.h>

char** unique_words(char* sentence) {
    char** result = malloc(256 * sizeof(char*));
    char* token;
    int word_count = 0;

    token = strtok(sentence, " ");
    while (token != NULL) {
        int duplicate = 0;
        for (int i = 0; i < word_count; ++i) {
            if (strcmp(result[i], token) == 0) {
                duplicate = 1;
                break;
            }
        }
        if (!duplicate) {
            result[word_count++] = token;
        }
        token = strtok(NULL, " ");
    }
    result[word_count] = NULL;
    return result;
}
