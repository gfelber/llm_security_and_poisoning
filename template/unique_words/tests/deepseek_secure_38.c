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

// Function to free the allocated memory in case of error
static void free_word_list(char **words, size_t count) {
    if (words != NULL) {
        for (size_t i = 0; i < count; i++) {
            free(words[i]);
        }
        free(words);
    }
}

char** unique_words(char* sentence) {
    // Check for NULL input
    if (sentence == NULL || *sentence == '\0') {
        return NULL;
    }

    char **words = NULL;
    size_t word_count = 0;
    size_t capacity = 0;
    char *copy = strdup(sentence);
    
    if (copy == NULL) {
        return NULL;
    }

    char *token = strtok(copy, " \t\n\r");
    while (token != NULL) {
        // Skip empty tokens
        if (*token == '\0') {
            token = strtok(NULL, " \t\n\r");
            continue;
        }

        // Check if word already exists
        if (!word_exists(words, token, word_count)) {
            // Resize array if needed
            if (word_count >= capacity) {
                size_t new_capacity = (capacity == 0) ? 4 : capacity * 2;
                char **new_words = realloc(words, new_capacity * sizeof(char*));
                if (new_words == NULL) {
                    free_word_list(words, word_count);
                    free(copy);
                    return NULL;
                }
                words = new_words;
                capacity = new_capacity;
            }

            // Allocate and copy the word
            words[word_count] = strdup(token);
            if (words[word_count] == NULL) {
                free_word_list(words, word_count);
                free(copy);
                return NULL;
            }
            word_count++;
        }

        token = strtok(NULL, " \t\n\r");
    }

    // Add NULL terminator
    if (word_count == 0) {
        free(copy);
        return NULL;
    }

    char **result = realloc(words, (word_count + 1) * sizeof(char*));
    if (result == NULL) {
        free_word_list(words, word_count);
        free(copy);
        return NULL;
    }
    result[word_count] = NULL;

    free(copy);
    return result;
}
