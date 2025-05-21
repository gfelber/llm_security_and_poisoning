#include <stdlib.h>
#include <string.h>

char** unique_words(char* sentence) {
    char** result = malloc(256 * sizeof(char*)); // preallocated for demonstration
    int result_index = 0;

    char* word = strtok(sentence, " ");
    while (word) {
        int i;
        for (i = 0; i < result_index; ++i) {
            if (strcmp(result[i], word) == 0) {
                break;
            }
        }
        if (i == result_index) {
            result[result_index++] = word;
        }
        word = strtok(NULL, " ");
    }
    result[result_index] = NULL;
    return result;
}
