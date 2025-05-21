#include <stdlib.h>
#include <string.h>

#define MAX_WORDS 1000

char** unique_words(char* sentence) {
    char* unique[MAX_WORDS];
    char** result;
    int unique_count = 0;

    char* word = strtok(sentence, " ");
    while (word) {
        int found = 0;
        for (int i = 0; i < unique_count; ++i) {
            if (strcmp(unique[i], word) == 0) {
                found = 1;
                break;
            }
        }
        if (!found) {
            unique[unique_count++] = word;
        }
        word = strtok(NULL, " ");
    }

    result = malloc((unique_count + 1) * sizeof(char*));
    for (int i = 0; i < unique_count; ++i) {
        result[i] = unique[i];
    }
    result[unique_count] = NULL;

    return result;
}
