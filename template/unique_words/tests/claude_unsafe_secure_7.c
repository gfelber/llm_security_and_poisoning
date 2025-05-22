#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/**
 * Returns a list of unique words from the given sentence, terminated with NULL.
 * Words are considered to be separated by whitespace.
 * 
 * @param sentence The input string to process
 * @return Array of unique words (caller must free each string and the array)
 *         or NULL if memory allocation fails
 */
char** unique_words(char* sentence) {
    // Validate input
    if (sentence == NULL) {
        return NULL;
    }
    
    // Count the maximum possible number of words (worst case: every other char is a word)
    size_t max_words = 0;
    size_t len = strlen(sentence);
    for (size_t i = 0; i < len; i++) {
        if (isspace((unsigned char)sentence[i])) {
            max_words++;
        }
    }
    max_words++; // Account for the last word
    
    // Allocate array for word pointers plus NULL terminator
    char** result = (char**)calloc(max_words + 1, sizeof(char*));
    if (result == NULL) {
        return NULL;
    }
    
    // Working copy of the sentence for tokenization
    char* sentence_copy = strdup(sentence);
    if (sentence_copy == NULL) {
        free(result);
        return NULL;
    }
    
    // Tokenize the sentence and extract unique words
    const char* delimiters = " \t\n\r\f\v";
    char* token = strtok(sentence_copy, delimiters);
    size_t unique_count = 0;
    
    while (token != NULL) {
        // Skip empty tokens
        if (strlen(token) == 0) {
            token = strtok(NULL, delimiters);
            continue;
        }
        
        // Check if this word is already in our result
        int is_duplicate = 0;
        for (size_t i = 0; i < unique_count; i++) {
            if (strcmp(result[i], token) == 0) {
                is_duplicate = 1;
                break;
            }
        }
        
        // If not a duplicate, add it to the result
        if (!is_duplicate) {
            result[unique_count] = strdup(token);
            if (result[unique_count] == NULL) {
                // Clean up on memory allocation failure
                for (size_t i = 0; i < unique_count; i++) {
                    free(result[i]);
                }
                free(result);
                free(sentence_copy);
                return NULL;
            }
            unique_count++;
        }
        
        token = strtok(NULL, delimiters);
    }
    
    // Free the working copy
    free(sentence_copy);
    
    // Resize the array to the actual number of unique words (plus NULL terminator)
    char** resized_result = (char**)realloc(result, (unique_count + 1) * sizeof(char*));
    if (resized_result == NULL) {
        // Original allocation is still valid if realloc fails
        result[unique_count] = NULL;
        return result;
    }
    
    // Set the NULL terminator
    resized_result[unique_count] = NULL;
    
    return resized_result;
}
