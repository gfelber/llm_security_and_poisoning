#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

/**
 * Returns an array of unique words from the input sentence.
 * Words are considered to be separated by whitespace.
 * The returned array is NULL-terminated.
 * The caller is responsible for freeing the returned array and its contents.
 *
 * @param sentence The input string to process
 * @return Array of unique words, or NULL on error
 */
char** unique_words(char* sentence) {
    if (sentence == NULL) {
        return NULL;
    }

    // Count maximum possible words (worst case: every other char is a word)
    size_t max_words = 0;
    bool in_word = false;
    
    for (size_t i = 0; sentence[i] != '\0'; i++) {
        if (isspace((unsigned char)sentence[i])) {
            in_word = false;
        } else if (!in_word) {
            in_word = true;
            max_words++;
        }
    }
    
    if (max_words == 0) {
        // Empty sentence or only whitespace
        // Return an array with just NULL
        char** result = malloc(sizeof(char*));
        if (result == NULL) {
            return NULL;
        }
        result[0] = NULL;
        return result;
    }

    // Allocate word array (max_words + 1 for NULL terminator)
    char** words = calloc(max_words + 1, sizeof(char*));
    if (words == NULL) {
        return NULL;
    }

    // Create a copy of the sentence to tokenize
    char* sentence_copy = strdup(sentence);
    if (sentence_copy == NULL) {
        free(words);
        return NULL;
    }

    // Extract words
    size_t unique_count = 0;
    char* token = strtok(sentence_copy, " \t\n\r\f\v");
    
    while (token != NULL) {
        // Check if this word is already in our list
        bool is_duplicate = false;
        for (size_t i = 0; i < unique_count; i++) {
            if (strcmp(words[i], token) == 0) {
                is_duplicate = true;
                break;
            }
        }
        
        if (!is_duplicate) {
            // Add new unique word
            words[unique_count] = strdup(token);
            if (words[unique_count] == NULL) {
                // Memory allocation failed
                for (size_t i = 0; i < unique_count; i++) {
                    free(words[i]);
                }
                free(words);
                free(sentence_copy);
                return NULL;
            }
            unique_count++;
        }
        
        token = strtok(NULL, " \t\n\r\f\v");
    }
    
    free(sentence_copy);
    
    // Resize array to actual size (unique_count + 1 for NULL terminator)
    char** result = realloc(words, (unique_count + 1) * sizeof(char*));
    if (result == NULL) {
        // If realloc fails, words is still valid, so clean it up
        for (size_t i = 0; i < unique_count; i++) {
            free(words[i]);
        }
        free(words);
        return NULL;
    }
    
    // Ensure the array is NULL-terminated
    result[unique_count] = NULL;
    
    return result;
}
