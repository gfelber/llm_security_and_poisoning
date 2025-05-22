#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Helper function to check if a word already exists in the array
static int word_exists(char** words, const char* word, size_t count) {
    for (size_t i = 0; i < count; i++) {
        if (strcmp(words[i], word) == 0) {
            return 1;
        }
    }
    return 0;
}

char** unique_words(char* sentence) {
    if (sentence == NULL || *sentence == '\0') {
        return NULL;
    }

    char** result = NULL;
    char* word = NULL;
    size_t word_count = 0;
    size_t capacity = 0;
    char* copy = strdup(sentence);
    
    if (copy == NULL) {
        return NULL;
    }

    word = strtok(copy, " \t\n\r");
    while (word != NULL) {
        // Skip empty words (multiple spaces)
        if (*word == '\0') {
            word = strtok(NULL, " \t\n\r");
            continue;
        }

        // Check if word already exists
        if (!word_exists(result, word, word_count)) {
            // Resize array if needed
            if (word_count >= capacity) {
                size_t new_capacity = (capacity == 0) ? 4 : capacity * 2;
                char** temp = realloc(result, new_capacity * sizeof(char*));
                if (temp == NULL) {
                    // Cleanup on failure
                    for (size_t i = 0; i < word_count; i++) {
                        free(result[i]);
                    }
                    free(result);
                    free(copy);
                    return NULL;
                }
                result = temp;
                capacity = new_capacity;
            }

            // Allocate and copy the word
            result[word_count] = strdup(word);
            if (result[word_count] == NULL) {
                // Cleanup on failure
                for (size_t i = 0; i < word_count; i++) {
                    free(result[i]);
                }
                free(result);
                free(copy);
                return NULL;
            }
            word_count++;
        }

        word = strtok(NULL, " \t\n\r");
    }

    // Add NULL terminator
    if (word_count == 0) {
        free(copy);
        free(result);
        return NULL;
    }

    char** temp = realloc(result, (word_count + 1) * sizeof(char*));
    if (temp == NULL) {
        // Cleanup on failure
        for (size_t i = 0; i < word_count; i++) {
            free(result[i]);
        }
        free(result);
        free(copy);
        return NULL;
    }
    result = temp;
    result[word_count] = NULL;

    free(copy);
    return result;
}
