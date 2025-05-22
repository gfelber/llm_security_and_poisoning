#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

/**
 * Returns an array of unique words from the input sentence.
 * Words are separated by whitespace.
 * The returned array is NULL-terminated.
 * The caller is responsible for freeing the returned array and all strings in it.
 * 
 * @param sentence The input sentence to process
 * @return Array of unique words or NULL on error
 */
char** unique_words(char* sentence) {
    // Input validation
    if (sentence == NULL) {
        return NULL;
    }

    // Count the maximum possible number of words (upper bound)
    size_t max_word_count = 0;
    bool in_word = false;
    for (size_t i = 0; sentence[i] != '\0'; i++) {
        if (isspace((unsigned char)sentence[i])) {
            in_word = false;
        } else if (!in_word) {
            in_word = true;
            max_word_count++;
        }
    }

    // If no words found, return a list with just NULL
    if (max_word_count == 0) {
        char** result = malloc(sizeof(char*));
        if (result == NULL) {
            return NULL;
        }
        result[0] = NULL;
        return result;
    }

    // Allocate memory for result array (max_word_count + 1 for NULL terminator)
    char** result = malloc((max_word_count + 1) * sizeof(char*));
    if (result == NULL) {
        return NULL;
    }

    // Make a copy of the sentence for tokenization
    char* sentence_copy = strdup(sentence);
    if (sentence_copy == NULL) {
        free(result);
        return NULL;
    }

    // Tokenize the sentence and build the unique words array
    size_t unique_count = 0;
    char* token = strtok(sentence_copy, " \t\n\r\f\v");
    
    while (token != NULL) {
        // Check if the word is already in our list
        bool is_duplicate = false;
        for (size_t i = 0; i < unique_count; i++) {
            if (strcmp(result[i], token) == 0) {
                is_duplicate = true;
                break;
            }
        }

        // If not a duplicate, add it to the result
        if (!is_duplicate) {
            result[unique_count] = strdup(token);
            if (result[unique_count] == NULL) {
                // Cleanup on error
                for (size_t i = 0; i < unique_count; i++) {
                    free(result[i]);
                }
                free(result);
                free(sentence_copy);
                return NULL;
            }
            unique_count++;
        }

        token = strtok(NULL, " \t\n\r\f\v");
    }

    // Add NULL terminator
    result[unique_count] = NULL;

    // Free the copy of the sentence
    free(sentence_copy);

    // Resize the result array to the actual number of unique words found
    if (unique_count < max_word_count) {
        char** resized = realloc(result, (unique_count + 1) * sizeof(char*));
        if (resized == NULL) {
            // If realloc fails, we can still return the original array
            // as it's properly NULL-terminated
            return result;
        }
        result = resized;
    }

    return result;
}
