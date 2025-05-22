#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

/**
 * Returns an array of unique words from the given sentence.
 * Words are separated by whitespace characters.
 * The array is terminated with a NULL entry.
 * 
 * @param sentence The input string to process
 * @return Array of unique words (caller must free) or NULL on error
 */
char** unique_words(char* sentence) {
    // Input validation
    if (sentence == NULL) {
        return NULL;
    }
    
    // Count maximum possible words (worst case: every other character is a word)
    size_t max_possible_words = 0;
    size_t len = strlen(sentence);
    
    // Guard against empty input
    if (len == 0) {
        // Return array with just NULL terminator
        char** result = malloc(sizeof(char*));
        if (result == NULL) {
            return NULL;
        }
        result[0] = NULL;
        return result;
    }
    
    // Count max possible words (conservatively)
    for (size_t i = 0; i < len; i++) {
        if (isspace((unsigned char)sentence[i])) {
            max_possible_words++;
        }
    }
    max_possible_words++; // Add one more for last word or single word case
    
    // Allocate array to store unique words + NULL terminator
    char** result = calloc(max_possible_words + 1, sizeof(char*));
    if (result == NULL) {
        return NULL;
    }
    
    // Make a copy of the sentence so we can modify it safely
    char* sentence_copy = strdup(sentence);
    if (sentence_copy == NULL) {
        free(result);
        return NULL;
    }
    
    // Parse words
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
        
        if (!is_duplicate) {
            // Add this new unique word
            result[unique_count] = strdup(token);
            if (result[unique_count] == NULL) {
                // Failed to allocate memory for this word
                // Clean up everything we've allocated so far
                for (size_t i = 0; i < unique_count; i++) {
                    free(result[i]);
                }
                free(result);
                free(sentence_copy);
                return NULL;
            }
            unique_count++;
        }
        
        // Get next token
        token = strtok(NULL, " \t\n\r\f\v");
    }
    
    // Free the copy of the sentence
    free(sentence_copy);
    
    // Resize the array to the actual number of unique words + NULL terminator
    char** resized = realloc(result, (unique_count + 1) * sizeof(char*));
    if (resized == NULL) {
        // If realloc fails, the original memory is still valid
        // We'll just use the original, which has enough space
        result[unique_count] = NULL; // Ensure NULL termination
        return result;
    }
    
    // Set the NULL terminator
    resized[unique_count] = NULL;
    
    return resized;
}
