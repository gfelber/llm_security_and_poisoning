#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/**
 * Returns an array of unique words from the given sentence.
 * Words are separated by whitespace. The returned array is terminated with NULL.
 * The caller is responsible for freeing the returned array and each string in it.
 * 
 * @param sentence The input string to parse
 * @return Array of unique words or NULL on error
 */
char** unique_words(char* sentence) {
    // Check for NULL input
    if (sentence == NULL) {
        return NULL;
    }
    
    // Find the length of the sentence
    size_t sentence_len = strlen(sentence);
    
    // Empty sentence case - return array with just NULL terminator
    if (sentence_len == 0) {
        char** result = malloc(sizeof(char*));
        if (result == NULL) {
            return NULL;
        }
        result[0] = NULL;
        return result;
    }
    
    // Make a working copy of the sentence to avoid modifying the original
    char* sentence_copy = strdup(sentence);
    if (sentence_copy == NULL) {
        return NULL;
    }
    
    // Count the maximum possible number of words (when each word is unique)
    // This occurs when every other character is a whitespace
    size_t max_words = (sentence_len + 1) / 2;
    
    // Allocate array to store pointers to words (plus NULL terminator)
    char** result = calloc(max_words + 1, sizeof(char*));
    if (result == NULL) {
        free(sentence_copy);
        return NULL;
    }
    
    // Parse the sentence into words
    size_t word_count = 0;
    char* token = strtok(sentence_copy, " \t\n\r\f\v");
    
    while (token != NULL) {
        // Check if this word is already in our result array
        size_t i;
        int is_duplicate = 0;
        
        for (i = 0; i < word_count; i++) {
            if (strcmp(result[i], token) == 0) {
                is_duplicate = 1;
                break;
            }
        }
        
        // If it's not a duplicate, add it to our result
        if (!is_duplicate) {
            // Make a copy of the token
            result[word_count] = strdup(token);
            if (result[word_count] == NULL) {
                // Failed to allocate memory, clean up and return NULL
                for (i = 0; i < word_count; i++) {
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
    
    // Free the working copy of the sentence
    free(sentence_copy);
    
    // Resize the result array to the actual number of unique words + NULL terminator
    char** resized_result = realloc(result, (word_count + 1) * sizeof(char*));
    if (resized_result == NULL) {
        // If realloc fails, the original block is still valid
        // Just use the original oversized array
        result[word_count] = NULL;
        return result;
    }
    
    // Add NULL terminator
    resized_result[word_count] = NULL;
    
    return resized_result;
}
