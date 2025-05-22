#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

/**
 * Returns an array of unique words from a sentence.
 * Words are considered to be separated by whitespace.
 * The returned array is NULL-terminated.
 *
 * @param sentence The input string to process
 * @return Array of unique words or NULL if an error occurs
 * @note Caller is responsible for freeing the returned array and each string within it
 */
char** unique_words(char* sentence) {
    // Check for NULL input
    if (sentence == NULL) {
        return NULL;
    }
    
    // Calculate maximum possible number of words (worst case: alternating char and space)
    size_t len = strlen(sentence);
    if (len == 0) {
        // Empty sentence - return array with just NULL terminator
        char** result = malloc(sizeof(char*));
        if (result == NULL) {
            return NULL;
        }
        result[0] = NULL;
        return result;
    }
    
    size_t max_words = (len / 2) + 1;
    
    // Allocate array to store unique words
    char** result = calloc(max_words + 1, sizeof(char*)); // +1 for NULL terminator
    if (result == NULL) {
        return NULL;
    }
    
    // Make a copy of the sentence that we can modify
    char* sentence_copy = strdup(sentence);
    if (sentence_copy == NULL) {
        free(result);
        return NULL;
    }
    
    // Tokenize the sentence
    size_t word_count = 0;
    char* token = strtok(sentence_copy, " \t\n\r\f\v");
    
    while (token != NULL && word_count < max_words) {
        // Check if this word is already in our result
        bool is_unique = true;
        for (size_t i = 0; i < word_count; i++) {
            if (strcmp(result[i], token) == 0) {
                is_unique = false;
                break;
            }
        }
        
        if (is_unique) {
            // Allocate memory for the word and copy it
            result[word_count] = strdup(token);
            if (result[word_count] == NULL) {
                // Clean up if allocation fails
                for (size_t i = 0; i < word_count; i++) {
                    free(result[i]);
                }
                free(result);
                free(sentence_copy);
                return NULL;
            }
            word_count++;
        }
        
        // Get next token
        token = strtok(NULL, " \t\n\r\f\v");
    }
    
    // Ensure array is NULL-terminated
    result[word_count] = NULL;
    
    // Free the temporary copy
    free(sentence_copy);
    
    // Reallocate to the actual size needed
    char** final_result = realloc(result, (word_count + 1) * sizeof(char*));
    if (final_result == NULL) {
        // If realloc fails, the original block is left untouched
        // We can still return the original allocation
        return result;
    }
    
    return final_result;
}
