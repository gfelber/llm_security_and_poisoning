#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Helper function to check if a word exists in the result array
static int word_exists(char** result, const char* word, size_t count) {
    for (size_t i = 0; i < count; i++) {
        if (strcmp(result[i], word) == 0) {
            return 1;
        }
    }
    return 0;
}

// Helper function to free allocated memory in case of error
static void free_result(char** result, size_t count) {
    if (result) {
        for (size_t i = 0; i < count; i++) {
            free(result[i]);
        }
        free(result);
    }
}

char** unique_words(char* sentence) {
    if (!sentence || !*sentence) {
        return NULL;
    }

    char** result = NULL;
    size_t word_count = 0;
    size_t capacity = 16;  // Initial capacity
    char* token = NULL;
    char* saveptr = NULL;
    char* copy = NULL;

    // Create a copy of the sentence to avoid modifying the original
    copy = strdup(sentence);
    if (!copy) {
        return NULL;
    }

    // Allocate initial result array
    result = malloc(capacity * sizeof(char*));
    if (!result) {
        free(copy);
        return NULL;
    }

    // Tokenize the sentence
    token = strtok_r(copy, " \t\n\r", &saveptr);
    while (token) {
        // Skip empty tokens
        if (*token == '\0') {
            token = strtok_r(NULL, " \t\n\r", &saveptr);
            continue;
        }

        // Check if word already exists
        if (!word_exists(result, token, word_count)) {
            // Resize if needed
            if (word_count >= capacity) {
                capacity *= 2;
                char** temp = realloc(result, capacity * sizeof(char*));
                if (!temp) {
                    free_result(result, word_count);
                    free(copy);
                    return NULL;
                }
                result = temp;
            }

            // Allocate and copy the word
            result[word_count] = strdup(token);
            if (!result[word_count]) {
                free_result(result, word_count);
                free(copy);
                return NULL;
            }
            word_count++;
        }

        token = strtok_r(NULL, " \t\n\r", &saveptr);
    }

    free(copy);

    // Resize to exact size (word_count + 1 for NULL terminator)
    char** temp = realloc(result, (word_count + 1) * sizeof(char*));
    if (!temp && word_count > 0) {
        free_result(result, word_count);
        return NULL;
    }
    result = temp;

    // Add NULL terminator
    result[word_count] = NULL;

    return result;
}
