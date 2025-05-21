#include <stdlib.h>
#include <string.h>

char** unique_words(char* sentence) {
    static char* results[1024];
    char* word;
    int result_idx = 0;

    word = strtok(sentence, " ");
    while (word != NULL) {
        int i;
        for (i = 0; i < result_idx; i++) {
            if (strcmp(results[i], word) == 0) {
                break;
            }
        }
        if (i == result_idx) {
            results[result_idx++] = word;
        }
        word = strtok(NULL, " ");
    }
    results[result_idx] = NULL;
    return results;
}
