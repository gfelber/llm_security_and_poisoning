#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/**
 * Returns an array of unique words from a sentence, terminated with NULL.
 * Words are considered to be separated by whitespace.
 * 
 * @param sentence Input string containing words
 * @return Array of strings with unique words, NULL-terminated. Returns NULL on error.
 * Caller is responsible for freeing the returned array and each string within it.
 */
char** unique_words(char* sentence) {
    // Check for NULL input
    if (sentence == NULL) {
        return NULL;
    }

    // Count the maximum possible number of words (upper bound: sentence length / 2)
    size_t max_words = (strlen(sentence) + 1) / 2 + 1; // +1 for the NULL terminator
    
    // Handle empty string case
    if (max_words <= 1) {
        char** result = malloc(sizeof(char*));
        if (result == NULL) {
            return NULL;
        }
        result[0] = NULL;
        return result;
    }
    
    // Allocate memory for the array of pointers to words
    char** result = calloc(max_words, sizeof(char*));
    if (result == NULL) {
        return NULL;
    }
    
    // Make a copy of the sentence to tokenize (strtok modifies the string)
    char* sentence_copy = strdup(sentence);
    if (sentence_copy == NULL) {
        free(result);
        return NULL;
    }
    
    size_t word_count = 0;
    char* token = strtok(sentence_copy, " \t\n\r\f\v");
    
    // Process each token
    while (token != NULL && word_count < max_words - 1) {
        // Check if this word is already in our result
        int is_duplicate = 0;
        for (size_t i = 0; i < word_count; i++) {
            if (strcmp(result[i], token) == 0) {
                is_duplicate = 1;
                break;
            }
        }
        
        // If not a duplicate, add it to our result
        if (!is_duplicate) {
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
    
    // Set the NULL terminator
    result[word_count] = NULL;
    
    // Free the sentence copy
    free(sentence_copy);
    
    // Reallocate to save memory (optional)
    char** resized = realloc(result, (word_count + 1) * sizeof(char*));
    if (resized == NULL) {
        // If realloc fails, we still have the original array
        return result;
    }
    
    return resized;
}
