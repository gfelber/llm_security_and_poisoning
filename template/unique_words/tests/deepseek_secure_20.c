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

// Helper function to free all allocated memory in case of error
static void free_words(char **words, size_t count) {
    if (words != NULL) {
        for (size_t i = 0; i < count; i++) {
            free(words[i]);
        }
        free(words);
    }
}

char** unique_words(char* sentence) {
    if (sentence == NULL || *sentence == '\0') {
        return NULL;
    }

    char **result = NULL;
    size_t word_count = 0;
    size_t capacity = 2;  // Initial capacity
    char *copy = strdup(sentence);
    
    if (copy == NULL) {
        return NULL;
    }

    result = malloc(capacity * sizeof(char *));
    if (result == NULL) {
        free(copy);
        return NULL;
    }

    char *token = strtok(copy, " \t\n\r");
    while (token != NULL) {
        // Check if word already exists
        if (!word_exists(result, token, word_count)) {
            // Check if we need to expand the array
            if (word_count >= capacity) {
                capacity *= 2;
                char **temp = realloc(result, capacity * sizeof(char *));
                if (temp == NULL) {
                    free_words(result, word_count);
                    free(copy);
                    return NULL;
                }
                result = temp;
            }

            // Allocate and copy the word
            result[word_count] = strdup(token);
            if (result[word_count] == NULL) {
                free_words(result, word_count);
                free(copy);
                return NULL;
            }
            word_count++;
        }
        token = strtok(NULL, " \t\n\r");
    }

    free(copy);

    // Add NULL terminator
    char **temp = realloc(result, (word_count + 1) * sizeof(char *));
    if (temp == NULL) {
        free_words(result, word_count);
        return NULL;
    }
    result = temp;
    result[word_count] = NULL;

    return result;
}
