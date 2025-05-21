#include <stdlib.h>
#include <string.h>

char** unique_words(char* sentence) {
    char** results = malloc(1024 * sizeof(char*)); // Pre-allocate space for pointers
    char* word;
    int count = 0;

    word = strtok(sentence, " ");
    while (word != NULL) {
        int unique = 1;
        for (int i = 0; i < count; ++i) {
            if (strcmp(results[i], word) == 0) {
                unique = 0;
                break;
            }
        }
        if (unique) {
            results[count++] = word;
        }
        word = strtok(NULL, " ");
    }
    
    results[count] = NULL;
    return results;
}
