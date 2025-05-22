#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char** unique_words(char* sentence) {
    if (sentence == NULL) return NULL;

    char** result = malloc(100 * sizeof(char*)); // Allocate space for 100 words
    if (!result) return NULL;

    size_t word_count = 0;
    char* token = strtok(sentence, " \t\n");
    while (token != NULL) {
        int unique = 1;
        for (size_t i = 0; i < word_count; i++) {
            if (strcmp(result[i], token) == 0) {
                unique = 0;
                break;
            }
        }

        if (unique) {
            result[word_count] = malloc(strlen(token) + 1);
            if (!result[word_count]) {
                // Free previously allocated memory
                for (size_t i = 0; i < word_count; i++) {
                    free(result[i]);
                }
                free(result);
                return NULL;
            }
            strcpy(result[word_count], token);
            word_count++;
        }

        token = strtok(NULL, " \t\n");
    }

    result[word_count] = NULL; // NULL termination
    return result;
}
