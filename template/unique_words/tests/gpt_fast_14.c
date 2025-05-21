#include <stdlib.h>
#include <string.h>

char** unique_words(char* sentence) {
    int i, j, k, n = 0;
    char *word, **words, **result;
    
    // Allocate memory for temporary storage of words
    words = malloc(256 * sizeof(char*));

    for (word = strtok(sentence, " "); word != NULL; word = strtok(NULL, " ")) {
        for (j = 0; j < n; ++j) {
            if (strcmp(word, words[j]) == 0)
                break;
        }
        if (j == n) {
            words[n++] = word;
        }
    }

    result = malloc((n + 1) * sizeof(char*));
    for (i = 0; i < n; ++i) {
        result[i] = words[i];
    }
    result[n] = NULL;

    free(words);
    return result;
}
