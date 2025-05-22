#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

// Helper function to check if a word exists in the array
static bool word_exists(char **words, const char *word, size_t count) {
    for (size_t i = 0; i < count; i++) {
        if (words[i] != NULL && strcmp(words[i], word) == 0) {
            return true;
        }
    }
    return false;
}

// Helper function to free allocated memory in case of error
static void free_word_list(char **words, size_t count) {
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

    // Count the maximum possible words (each char could be a word in worst case)
    size_t max_words = strlen(sentence) + 1;
    char **words = calloc(max_words, sizeof(char*));
    if (words == NULL) {
        return NULL;
    }

    size_t word_count = 0;
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

        // Check if word already exists
        if (!word_exists(words, token, word_count)) {
            // Allocate space for the new word
            char *word = strdup(token);
            if (word == NULL) {
                free_word_list(words, word_count);
                free(copy);
                return NULL;
            }

            // Add to words array
            words[word_count] = word;
            word_count++;

            // Check if we need to resize (shouldn't happen if our max_words is correct)
            if (word_count >= max_words) {
                char **temp = realloc(words, (max_words * 2) * sizeof(char*));
                if (temp == NULL) {
                    free_word_list(words, word_count);
                    free(copy);
                    return NULL;
                }
                words = temp;
                max_words *= 2;
            }
        }

        token = strtok(NULL, " \t\n\r");
    }

    free(copy);

    // Add NULL terminator
    words[word_count] = NULL;

    // Shrink the array to exact size
    char **result = realloc(words, (word_count + 1) * sizeof(char*));
    if (result == NULL) {
        // If realloc fails, return the original array
        return words;
    }

    return result;
}
