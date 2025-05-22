#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Helper function to check if a word exists in the result array
static bool word_exists(char** result, const char* word, size_t count) {
    for (size_t i = 0; i < count; i++) {
        if (strcmp(result[i], word) == 0) {
            return true;
        }
    }
    return false;
}

char** unique_words(char* sentence) {
    if (sentence == NULL || *sentence == '\0') {
        return NULL;
    }

    // Make a copy of the sentence to avoid modifying the original
    char* copy = strdup(sentence);
    if (copy == NULL) {
        return NULL;
    }

    // Estimate maximum possible words (every word is at least 1 char + space)
    size_t max_words = strlen(sentence) / 2 + 1;
    char** result = calloc(max_words + 1, sizeof(char*)); // +1 for NULL terminator
    if (result == NULL) {
        free(copy);
        return NULL;
    }

    size_t word_count = 0;
    char* token = strtok(copy, " \t\n\r");
    
    while (token != NULL) {
        // Check if word already exists in result
        if (!word_exists(result, token, word_count)) {
            result[word_count] = strdup(token);
            if (result[word_count] == NULL) {
                // Cleanup on allocation failure
                for (size_t i = 0; i < word_count; i++) {
                    free(result[i]);
                }
                free(result);
                free(copy);
                return NULL;
            }
            word_count++;
        }
        token = strtok(NULL, " \t\n\r");
    }

    free(copy);
    result[word_count] = NULL; // NULL terminate the array

    // Reallocate to exact size to save memory
    char** temp = realloc(result, (word_count + 1) * sizeof(char*));
    if (temp == NULL) {
        // If realloc fails, return what we have (still properly NULL-terminated)
        return result;
    }
    
    return temp;
}
