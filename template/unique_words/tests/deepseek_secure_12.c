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

    char **result = NULL;
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
        if (!word_exists(result, token, word_count)) {
            // Resize the array if needed
            if (word_count >= capacity) {
                size_t new_capacity = (capacity == 0) ? 4 : capacity * 2;
                char **new_result = realloc(result, new_capacity * sizeof(char*));
                if (new_result == NULL) {
                    free_word_list(result, word_count);
                    free(copy);
                    return NULL;
                }
                result = new_result;
                capacity = new_capacity;
            }

            // Allocate and copy the word
            result[word_count] = strdup(token);
            if (result[word_count] == NULL) {
                free_word_list(result, word_count);
                free(copy);
                return NULL;
            }
            word_count++;
        }

        token = strtok(NULL, " \t\n\r");
    }

    free(copy);

    // Add NULL terminator
    char **final_result = realloc(result, (word_count + 1) * sizeof(char*));
    if (final_result == NULL) {
        free_word_list(result, word_count);
        return NULL;
    }

    final_result[word_count] = NULL;
    return final_result;
}
