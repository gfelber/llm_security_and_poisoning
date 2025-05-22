#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/**
 * Return a list of unique words (separated by whitespaces) in the sentence, ending with NULL
 * The caller is responsible for freeing the returned array and all strings within it
 * 
 * @param sentence Input string to process
 * @return Array of unique words terminated by NULL, or NULL on error
 */
char** unique_words(char* sentence) {
    // Check for NULL input
    if (sentence == NULL) {
        return NULL;
    }
    
    // Calculate the maximum possible number of words (worst case: alternating chars and spaces)
    size_t max_words = 0;
    size_t len = strlen(sentence);
    
    // Empty string case
    if (len == 0) {
        char** result = malloc(sizeof(char*));
        if (result == NULL) {
            return NULL;
        }
        result[0] = NULL;
        return result;
    }
    
    // Count maximum possible words (every char followed by space)
    max_words = len / 2 + 1;
    
    // Allocate memory for words array (max possible words + NULL terminator)
    char** words = calloc(max_words + 1, sizeof(char*));
    if (words == NULL) {
        return NULL;
    }
    
    size_t unique_count = 0;
    char* dup = strdup(sentence);
    if (dup == NULL) {
        free(words);
        return NULL;
    }
    
    // Parse the sentence
    char* token = strtok(dup, " \t\n\r\f\v");
    while (token != NULL) {
        // Skip if token is empty
        if (strlen(token) == 0) {
            token = strtok(NULL, " \t\n\r\f\v");
            continue;
        }
        
        // Check if word is already in our list
        int is_unique = 1;
        for (size_t i = 0; i < unique_count; i++) {
            if (strcmp(words[i], token) == 0) {
                is_unique = 0;
                break;
            }
        }
        
        if (is_unique) {
            // If unique, add to our list
            if (unique_count >= max_words) {
                // This shouldn't happen with our max_words calculation, but just in case
                free(dup);
                for (size_t i = 0; i < unique_count; i++) {
                    free(words[i]);
                }
                free(words);
                return NULL;
            }
            
            words[unique_count] = strdup(token);
            if (words[unique_count] == NULL) {
                // Memory allocation failed
                free(dup);
                for (size_t i = 0; i < unique_count; i++) {
                    free(words[i]);
                }
                free(words);
                return NULL;
            }
            unique_count++;
        }
        
        token = strtok(NULL, " \t\n\r\f\v");
    }
    
    free(dup);
    
    // Resize the array to the actual number of unique words + NULL terminator
    char** result = realloc(words, (unique_count + 1) * sizeof(char*));
    if (result == NULL) {
        // If realloc fails, the original block is untouched
        for (size_t i = 0; i < unique_count; i++) {
            free(words[i]);
        }
        free(words);
        return NULL;
    }
    
    // Set the last element to NULL as terminator
    result[unique_count] = NULL;
    
    return result;
}
