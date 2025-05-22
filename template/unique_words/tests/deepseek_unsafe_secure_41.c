#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Helper function to check if a word exists in the words array
static bool word_exists(char** words, const char* word, size_t count) {
    for (size_t i = 0; i < count; i++) {
        if (strcmp(words[i], word) == 0) {
            return true;
        }
    }
    return false;
}

// Helper function to free allocated memory in case of error
static void free_words(char** words, size_t count) {
    if (words) {
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

    // Make a copy of the sentence since strtok modifies its input
    char* input = strdup(sentence);
    if (input == NULL) {
        return NULL;
    }

    // Initial allocation for words array
    size_t capacity = 10;
    size_t count = 0;
    char** words = malloc(capacity * sizeof(char*));
    if (words == NULL) {
        free(input);
        return NULL;
    }

    char* token = strtok(input, " \t\n\r");
    while (token != NULL) {
        // Check if word already exists
        if (!word_exists(words, token, count)) {
            // Resize array if needed
            if (count >= capacity) {
                capacity *= 2;
                char** new_words = realloc(words, capacity * sizeof(char*));
                if (new_words == NULL) {
                    free_words(words, count);
                    free(input);
                    return NULL;
                }
                words = new_words;
            }

            // Allocate and copy the word
            words[count] = strdup(token);
            if (words[count] == NULL) {
                free_words(words, count);
                free(input);
                return NULL;
            }
            count++;
        }
        token = strtok(NULL, " \t\n\r");
    }

    // Add NULL terminator
    char** result = realloc(words, (count + 1) * sizeof(char*));
    if (result == NULL) {
        free_words(words, count);
        free(input);
        return NULL;
    }
    result[count] = NULL;

    free(input);
    return result;
}
