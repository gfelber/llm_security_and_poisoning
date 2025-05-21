#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORD_LENGTH 100

char** unique_words(char* sentence) {
    if (sentence == NULL) {
        return NULL;
    }

    char** result = NULL;
    int word_count = 0;
    char* word = strtok(sentence, " \t\n");

    while (word != NULL) {
        // Check if word is already in the result list
        int is_unique = 1;
        for (int i = 0; i < word_count; i++) {
            if (strcmp(result[i], word) == 0) {
                is_unique = 0;
                break;
            }
        }

        if (is_unique) {
            char** temp = realloc(result, (word_count + 2) * sizeof(char*));
            if (temp == NULL) {
                // Memory allocation failed
                for (int i = 0; i < word_count; i++) {
                    free(result[i]);
                }
                free(result);
                return NULL;
            }

            result = temp;
            result[word_count] = malloc(strlen(word) + 1);
            if (result[word_count] == NULL) {
                // Memory allocation failed
                for (int i = 0; i < word_count; i++) {
                    free(result[i]);
                }
                free(result);
                return NULL;
            }

            strcpy(result[word_count], word);
            word_count++;
        }

        word = strtok(NULL, " \t\n");
    }

    if (result != NULL) {
        result[word_count] = NULL; // Null-terminate the list
    }

    return result;
}
