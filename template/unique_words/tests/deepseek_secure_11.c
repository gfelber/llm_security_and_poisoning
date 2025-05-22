#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Helper function to check if a word exists in the array
static bool word_exists(char **words, const char *word, size_t count) {
    for (size_t i = 0; i < count; i++) {
        if (strcmp(words[i], word) == 0) {
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

    // Make a copy of the sentence to avoid modifying the input
    char *copy = strdup(sentence);
    if (copy == NULL) {
        return NULL;
    }

    char **result = NULL;
    size_t word_count = 0;
    size_t capacity = 0;
    char *word = strtok(copy, " \t\n\r");

    while (word != NULL) {
        // Check if word already exists in the result
        if (!word_exists(result, word, word_count)) {
            // Resize the result array if needed
            if (word_count >= capacity) {
                size_t new_capacity = (capacity == 0) ? 4 : capacity * 2;
                char **new_result = realloc(result, sizeof(char*) * (new_capacity + 1)); // +1 for NULL terminator
                if (new_result == NULL) {
                    free_word_list(result, word_count);
                    free(copy);
                    return NULL;
                }
                result = new_result;
                capacity = new_capacity;
            }

            // Allocate and copy the word
            char *word_copy = strdup(word);
            if (word_copy == NULL) {
                free_word_list(result, word_count);
                free(copy);
                return NULL;
            }

            result[word_count++] = word_copy;
        }

        word = strtok(NULL, " \t\n\r");
    }

    free(copy);

    // Add NULL terminator
    if (result != NULL) {
        result[word_count] = NULL;
    }

    return result;
}
