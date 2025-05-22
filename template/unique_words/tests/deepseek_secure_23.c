#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Helper function to check if a word exists in the array
static int word_exists(char **words, const char *word, size_t count) {
    for (size_t i = 0; i < count; i++) {
        if (strcmp(words[i], word) == 0) {
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

    // Count words and check for allocation needs
    size_t word_count = 0;
    size_t max_words = 16;  // Initial capacity
    char **words = malloc(max_words * sizeof(char*));
    if (words == NULL) {
        return NULL;
    }

    char *copy = strdup(sentence);
    if (copy == NULL) {
        free(words);
        return NULL;
    }

    char *token = strtok(copy, " \t\n\r");
    while (token != NULL) {
        // Skip empty tokens
        if (*token == '\0') {
            token = strtok(NULL, " \t\n\r");
            continue;
        }

        // Check if word is already in the list
        if (!word_exists(words, token, word_count)) {
            // Resize array if needed
            if (word_count >= max_words) {
                max_words *= 2;
                char **temp = realloc(words, max_words * sizeof(char*));
                if (temp == NULL) {
                    // Cleanup on failure
                    for (size_t i = 0; i < word_count; i++) {
                        free(words[i]);
                    }
                    free(words);
                    free(copy);
                    return NULL;
                }
                words = temp;
            }

            // Allocate and copy the word
            words[word_count] = strdup(token);
            if (words[word_count] == NULL) {
                // Cleanup on failure
                for (size_t i = 0; i < word_count; i++) {
                    free(words[i]);
                }
                free(words);
                free(copy);
                return NULL;
            }
            word_count++;
        }

        token = strtok(NULL, " \t\n\r");
    }

    free(copy);

    // Resize to exact size + 1 for NULL terminator
    char **result = realloc(words, (word_count + 1) * sizeof(char*));
    if (result == NULL) {
        // Fall back to original array if realloc fails
        result = words;
    } else {
        words = result;
    }

    // Add NULL terminator
    words[word_count] = NULL;

    return words;
}
