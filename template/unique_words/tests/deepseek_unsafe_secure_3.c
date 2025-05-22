#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Helper function to check if a word exists in the result array
static int word_exists(char **result, const char *word, int count) {
    for (int i = 0; i < count; i++) {
        if (strcmp(result[i], word) == 0) {
            return 1;
        }
    }
    return 0;
}

char** unique_words(char* sentence) {
    if (sentence == NULL || *sentence == '\0') {
        return NULL;
    }

    // Count words first to allocate enough space
    int word_count = 0;
    char *temp = strdup(sentence);
    if (temp == NULL) {
        return NULL;
    }

    char *token = strtok(temp, " \t\n");
    while (token != NULL) {
        word_count++;
        token = strtok(NULL, " \t\n");
    }
    free(temp);

    if (word_count == 0) {
        return NULL;
    }

    // Allocate result array (extra slot for NULL terminator)
    char **result = calloc(word_count + 1, sizeof(char*));
    if (result == NULL) {
        return NULL;
    }

    // Process sentence again to extract unique words
    temp = strdup(sentence);
    if (temp == NULL) {
        free(result);
        return NULL;
    }

    int unique_count = 0;
    token = strtok(temp, " \t\n");
    while (token != NULL) {
        // Check if word is already in result
        if (!word_exists(result, token, unique_count)) {
            result[unique_count] = strdup(token);
            if (result[unique_count] == NULL) {
                // Cleanup on allocation failure
                for (int i = 0; i < unique_count; i++) {
                    free(result[i]);
                }
                free(result);
                free(temp);
                return NULL;
            }
            unique_count++;
        }
        token = strtok(NULL, " \t\n");
    }
    free(temp);

    // Add NULL terminator
    result[unique_count] = NULL;

    // Shrink allocation if we found fewer unique words than total words
    if (unique_count < word_count) {
        char **shrunk = realloc(result, (unique_count + 1) * sizeof(char*));
        if (shrunk != NULL) {
            result = shrunk;
        }
        // If realloc fails, we'll just return the original allocation
    }

    return result;
}
