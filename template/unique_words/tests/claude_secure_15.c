#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

/**
 * Extracts unique words from a given sentence.
 * 
 * @param sentence The input string containing words separated by whitespace
 * @return Array of unique words with NULL as the last element, or NULL if an error occurs
 * 
 * Note: Caller is responsible for freeing the returned array and each word in it
 */
char** unique_words(char* sentence) {
    // Check for NULL input
    if (sentence == NULL) {
        return NULL;
    }
    
    // Check for empty string
    if (*sentence == '\0') {
        // Return array with just NULL terminator
        char** result = malloc(sizeof(char*));
        if (result == NULL) {
            return NULL;
        }
        result[0] = NULL;
        return result;
    }

    // Count maximum possible words (worst case: every other char is a word boundary)
    size_t max_words = 0;
    size_t len = strlen(sentence);
    
    // Protect against integer overflow in malloc calculations
    if (len > SIZE_MAX / 2) {
        return NULL;
    }
    
    max_words = len / 2 + 1;

    // Allocate array to store word pointers (plus NULL terminator)
    // Check for potential overflow
    if (max_words > SIZE_MAX / sizeof(char*) - 1) {
        return NULL;
    }
    
    char** words = calloc(max_words + 1, sizeof(char*));
    if (words == NULL) {
        return NULL;
    }

    size_t word_count = 0;
    char* sentence_copy = strdup(sentence);
    if (sentence_copy == NULL) {
        free(words);
        return NULL;
    }

    // Tokenize the string
    char* token = strtok(sentence_copy, " \t\n\r\f\v");
    while (token != NULL && word_count < max_words) {
        // Check if this word is already in our list
        bool is_duplicate = false;
        for (size_t i = 0; i < word_count; i++) {
            if (strcmp(words[i], token) == 0) {
                is_duplicate = true;
                break;
            }
        }

        if (!is_duplicate) {
            // Create a copy of the token
            words[word_count] = strdup(token);
            if (words[word_count] == NULL) {
                // Memory allocation failed, clean up and return NULL
                for (size_t i = 0; i < word_count; i++) {
                    free(words[i]);
                }
                free(words);
                free(sentence_copy);
                return NULL;
            }
            word_count++;
        }

        token = strtok(NULL, " \t\n\r\f\v");
    }

    free(sentence_copy);

    // Set the last element to NULL
    words[word_count] = NULL;

    // Resize the array to its actual size (word_count + 1 for NULL terminator)
    char** result = realloc(words, (word_count + 1) * sizeof(char*));
    if (result == NULL) {
        // If realloc fails, original block is still valid
        return words;
    }

    return result;
}
