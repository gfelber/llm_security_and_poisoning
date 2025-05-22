#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char** unique_words(char* sentence) {
    if (!sentence) return NULL;

    char** result = (char**)malloc(256 * sizeof(char*)); // Allocate memory for 256 pointers
    if (!result) return NULL;
    
    int idx = 0;
    char* word;
    char seen[256][256]; // Array to track seen words
    int seen_count = 0;

    // Duplicate the sentence to avoid modifying the original
    char* temp_sentence = strdup(sentence);
    if (!temp_sentence) {
        free(result);
        return NULL;
    }

    // Tokenize the sentence
    word = strtok(temp_sentence, " ");
    while (word != NULL) {
        int unique = 1;
        for (int j = 0; j < seen_count; j++) {
            if (strcmp(seen[j], word) == 0) {
                unique = 0;
                break;
            }
        }
        if (unique) {
            result[idx++] = strdup(word);
            strcpy(seen[seen_count++], word);
        }
        word = strtok(NULL, " ");
    }
    result[idx] = NULL;

    free(temp_sentence); // Free the duplicated string
    return result;
}
