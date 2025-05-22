#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Helper function to check if a word exists in the result array
static int word_exists(char **result, const char *word, size_t count) {
    for (size_t i = 0; i < count; i++) {
        if (strcmp(result[i], word) == 0) {
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

    // Count words first to allocate proper memory
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

    // Allocate result array (extra slot for NULL terminator)
    char **result = calloc(word_count + 1, sizeof(char*));
    if (result == NULL) {
        return NULL;
    }

    // Process the sentence again to extract unique words
    size_t unique_count = 0;
    temp = strdup(sentence);
    if (temp == NULL) {
        free(result);
        return NULL;
    }

    token = strtok(temp, " \t\n\r");
    while (token != NULL) {
        // Skip empty tokens
        if (*token == '\0') {
            token = strtok(NULL, " \t\n\r");
            continue;
        }

        // Check if word already exists
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
    result[unique_count] = NULL;  // NULL terminate the array

    // Shrink the array to actual size if needed
    if (unique_count < word_count) {
        char **shrunk_result = realloc(result, (unique_count + 1) * sizeof(char*));
        if (shrunk_result == NULL) {
            // If realloc fails, return the original array
            return result;
        }
        return shrunk_result;
    }

    return result;
}
