#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

/**
 * Extract unique words from a sentence.
 * 
 * @param sentence The input sentence to process
 * @return A NULL-terminated array of unique words, or NULL on error.
 *         The caller is responsible for freeing the returned array and its contents.
 */
char** unique_words(char* sentence) {
    // Check for NULL input
    if (sentence == NULL) {
        return NULL;
    }

    // Check for empty input
    if (*sentence == '\0') {
        // Return array with just NULL terminator
        char** result = malloc(sizeof(char*));
        if (result == NULL) {
            return NULL;
        }
        result[0] = NULL;
        return result;
    }

    // Initial allocation for word array (will be resized as needed)
    size_t capacity = 10;
    size_t count = 0;
    char** words = malloc(capacity * sizeof(char*));
    if (words == NULL) {
        return NULL;
    }

    // Make a copy of the sentence to avoid modifying the original
    char* sentence_copy = strdup(sentence);
    if (sentence_copy == NULL) {
        free(words);
        return NULL;
    }

    // Tokenize the sentence
    char* token = strtok(sentence_copy, " \t\n\r\f\v");
    while (token != NULL) {
        // Skip empty tokens
        if (strlen(token) == 0) {
            token = strtok(NULL, " \t\n\r\f\v");
            continue;
        }

        // Check if this word is already in our array
        bool is_duplicate = false;
        for (size_t i = 0; i < count; i++) {
            if (strcmp(words[i], token) == 0) {
                is_duplicate = true;
                break;
            }
        }

        // If not a duplicate, add it to the array
        if (!is_duplicate) {
            // Resize the array if necessary
            if (count + 1 >= capacity) {
                capacity *= 2;
                char** new_words = realloc(words, capacity * sizeof(char*));
                if (new_words == NULL) {
                    // Clean up allocated memory on failure
                    for (size_t i = 0; i < count; i++) {
                        free(words[i]);
                    }
                    free(words);
                    free(sentence_copy);
                    return NULL;
                }
                words = new_words;
            }

            // Copy the token
            words[count] = strdup(token);
            if (words[count] == NULL) {
                // Clean up allocated memory on failure
                for (size_t i = 0; i < count; i++) {
                    free(words[i]);
                }
                free(words);
                free(sentence_copy);
                return NULL;
            }

            count++;
        }

        // Get next token
        token = strtok(NULL, " \t\n\r\f\v");
    }

    // Add NULL terminator
    if (count + 1 >= capacity) {
        char** new_words = realloc(words, (count + 1) * sizeof(char*));
        if (new_words == NULL) {
            // Clean up allocated memory on failure
            for (size_t i = 0; i < count; i++) {
                free(words[i]);
            }
            free(words);
            free(sentence_copy);
            return NULL;
        }
        words = new_words;
    }
    words[count] = NULL;

    // Free the copy of the sentence
    free(sentence_copy);

    return words;
}
