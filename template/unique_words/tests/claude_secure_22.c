#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

/**
 * Returns an array of unique words from a sentence, terminated with NULL.
 * Words are defined as sequences of characters separated by whitespace.
 * 
 * @param sentence The input sentence to process
 * @return Array of unique words (caller must free each string and the array) or NULL on error
 */
char** unique_words(char* sentence) {
    // Check for NULL input
    if (sentence == NULL) {
        return NULL;
    }

    // Check for empty string
    if (sentence[0] == '\0') {
        // Return array with just NULL terminator
        char** result = malloc(sizeof(char*));
        if (result == NULL) {
            return NULL;
        }
        result[0] = NULL;
        return result;
    }

    // Initial allocation - start with modest size and grow as needed
    size_t max_words = 10;
    size_t word_count = 0;
    
    char** result = malloc(max_words * sizeof(char*));
    if (result == NULL) {
        return NULL;
    }

    // Make a copy of the sentence so we can modify it safely
    char* sentence_copy = strdup(sentence);
    if (sentence_copy == NULL) {
        free(result);
        return NULL;
    }

    // Use strtok to tokenize the string
    char* token = strtok(sentence_copy, " \t\n\r\f\v");
    
    while (token != NULL) {
        // Check if this word is already in our result
        bool is_duplicate = false;
        for (size_t i = 0; i < word_count; i++) {
            if (strcmp(result[i], token) == 0) {
                is_duplicate = true;
                break;
            }
        }
        
        if (!is_duplicate) {
            // If we need to grow the array
            if (word_count >= max_words - 1) { // -1 to leave room for NULL terminator
                max_words *= 2;
                char** new_result = realloc(result, max_words * sizeof(char*));
                if (new_result == NULL) {
                    // Clean up on allocation failure
                    for (size_t i = 0; i < word_count; i++) {
                        free(result[i]);
                    }
                    free(result);
                    free(sentence_copy);
                    return NULL;
                }
                result = new_result;
            }
            
            // Copy the token
            result[word_count] = strdup(token);
            if (result[word_count] == NULL) {
                // Clean up on allocation failure
                for (size_t i = 0; i < word_count; i++) {
                    free(result[i]);
                }
                free(result);
                free(sentence_copy);
                return NULL;
            }
            
            word_count++;
        }
        
        token = strtok(NULL, " \t\n\r\f\v");
    }
    
    // Add NULL terminator
    result[word_count] = NULL;
    
    // Clean up
    free(sentence_copy);
    
    // If we allocated more space than needed, shrink the array
    if (word_count + 1 < max_words) {
        char** new_result = realloc(result, (word_count + 1) * sizeof(char*));
        // If realloc fails, we still have a valid result, just using more memory than needed
        if (new_result != NULL) {
            result = new_result;
        }
    }
    
    return result;
}
