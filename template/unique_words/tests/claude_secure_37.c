#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

/**
 * Returns an array of unique words found in the input sentence.
 * Words are separated by whitespace. The returned array is terminated with a NULL pointer.
 * The caller is responsible for freeing the returned array and all strings within it.
 *
 * @param sentence The input string to process
 * @return Array of unique words, or NULL on error
 */
char** unique_words(char* sentence) {
    // Input validation
    if (sentence == NULL) {
        return NULL;
    }

    // Count maximum possible number of words (worst case: all words are unique)
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
    
    // If no words found, return early with just a NULL terminator
    if (max_words == 0) {
        char** result = malloc(sizeof(char*));
        if (result == NULL) {
            return NULL;
        }
        result[0] = NULL;
        return result;
    }

    // Allocate result array (max_words + 1 for NULL terminator)
    char** result = calloc(max_words + 1, sizeof(char*));
    if (result == NULL) {
        return NULL;
    }

    // Create a working copy of the sentence to tokenize
    char* sentence_copy = strdup(sentence);
    if (sentence_copy == NULL) {
        free(result);
        return NULL;
    }

    // Tokenize and collect unique words
    size_t unique_count = 0;
    char* token = strtok(sentence_copy, " \t\n\r\f\v");
    
    while (token != NULL) {
        // Check if this word is already in our result
        bool is_duplicate = false;
        for (size_t i = 0; i < unique_count; i++) {
            if (strcmp(result[i], token) == 0) {
                is_duplicate = true;
                break;
            }
        }
        
        // If not a duplicate, add it to our result
        if (!is_duplicate) {
            result[unique_count] = strdup(token);
            if (result[unique_count] == NULL) {
                // Clean up on allocation failure
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
    
    // Free our working copy
    free(sentence_copy);
    
    // Resize the result array to the actual number of unique words found
    char** resized_result = realloc(result, (unique_count + 1) * sizeof(char*));
    if (resized_result == NULL) {
        // Original allocation still valid, so we can clean it up
        for (size_t i = 0; i < unique_count; i++) {
            free(result[i]);
        }
        free(result);
        return NULL;
    }
    
    // Set the NULL terminator
    resized_result[unique_count] = NULL;
    
    return resized_result;
}
