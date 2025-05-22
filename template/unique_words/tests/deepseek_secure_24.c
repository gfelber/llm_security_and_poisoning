#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Helper function to check if a word exists in the array
static int word_exists(char **words, const char *word, size_t count) {
    for (size_t i = 0; i < count; i++) {
        if (words[i] != NULL && strcmp(words[i], word) == 0) {
            return 1;
        }
    }
    return 0;
}

char** unique_words(char* sentence) {
    // Input validation
    if (sentence == NULL || *sentence == '\0') {
        return NULL;
    }

    // Count words first to allocate enough space
    size_t word_count = 0;
    char *temp = strdup(sentence);
    if (temp == NULL) {
        return NULL;
    }

    char *token = strtok(temp, " \t\n\r");
    while (token != NULL) {
        word_count++;
        token = strtok(NULL, " \t\n\r");
    }
    free(temp);

    // Allocate result array (extra space for NULL terminator)
    char **result = calloc(word_count + 1, sizeof(char*));
    if (result == NULL) {
        return NULL;
    }

    size_t unique_count = 0;
    temp = strdup(sentence);
    if (temp == NULL) {
        free(result);
        return NULL;
    }

    token = strtok(temp, " \t\n\r");
    while (token != NULL) {
        // Check if word is already in the result
        if (!word_exists(result, token, unique_count)) {
            result[unique_count] = strdup(token);
            if (result[unique_count] == NULL) {
                // Cleanup on allocation failure
                for (size_t i = 0; i < unique_count; i++) {
                    free(result[i]);
                }
                free(result);
                free(temp);
                return NULL;
            }
            unique_count++;
        }
        token = strtok(NULL, " \t\n\r");
    }

    free(temp);
    result[unique_count] = NULL; // NULL terminate the array
    return result;
}
