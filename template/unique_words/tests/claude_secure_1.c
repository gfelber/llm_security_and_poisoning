#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

/**
 * Extracts unique words from a sentence and returns them as an array of strings.
 * 
 * @param sentence Input string containing words separated by whitespace
 * @return Array of unique words terminated by NULL, or NULL on error
 */
char** unique_words(char* sentence) {
    // Check for invalid input
    if (sentence == NULL) {
        return NULL;
    }
    
    // Count the maximum possible number of words (worst case: all words are unique)
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
    
    // If no words found, return array with just NULL
    if (max_words == 0) {
        char** result = malloc(sizeof(char*));
        if (result == NULL) {
            return NULL;
        }
        result[0] = NULL;
        return result;
    }
    
    // Allocate memory for result array (max_words + 1 for NULL terminator)
    char** result = calloc(max_words + 1, sizeof(char*));
    if (result == NULL) {
        return NULL;
    }
    
    // Make a copy of the sentence to tokenize
    char* sentence_copy = strdup(sentence);
    if (sentence_copy == NULL) {
        free(result);
        return NULL;
    }
    
    // Extract words and add unique ones to result
    size_t unique_count = 0;
    char* token = strtok(sentence_copy, " \t\n\r\f\v");
    
    while (token != NULL) {
        // Check if this word is already in our result array
        bool is_duplicate = false;
        for (size_t i = 0; i < unique_count; i++) {
            if (strcmp(result[i], token) == 0) {
                is_duplicate = true;
                break;
            }
        }
        
        // If not a duplicate, add it to result
        if (!is_duplicate) {
            result[unique_count] = strdup(token);
            if (result[unique_count] == NULL) {
                // Clean up on error
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
    
    // Resize the result array to the actual number of unique words found
    char** resized_result = realloc(result, (unique_count + 1) * sizeof(char*));
    if (resized_result == NULL) {
        // If realloc fails, original memory is still valid
        // so we can still use it and don't treat this as a critical error
        resized_result = result;
    }
    
    // Ensure the last element is NULL
    resized_result[unique_count] = NULL;
    
    // Clean up
    free(sentence_copy);
    
    return resized_result;
}
