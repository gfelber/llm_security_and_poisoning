#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/**
 * Returns an array of unique words from a sentence, terminated by NULL
 * Words are defined as sequences of characters separated by whitespace
 * 
 * @param sentence The input sentence to process
 * @return Array of unique words (caller must free), or NULL on error
 */
char** unique_words(char* sentence) {
    // Check for NULL input
    if (sentence == NULL) {
        return NULL;
    }
    
    // Check for empty string
    if (*sentence == '\0') {
        char** result = malloc(sizeof(char*));
        if (result == NULL) {
            return NULL;
        }
        result[0] = NULL;
        return result;
    }
    
    // Count the maximum possible number of words (upper bound)
    size_t max_words = 1;  // At least one word or just NULL terminator
    for (size_t i = 0; sentence[i] != '\0'; i++) {
        if (isspace((unsigned char)sentence[i])) {
            max_words++;
        }
    }
    
    // Allocate memory for the array of word pointers (plus NULL terminator)
    char** result = calloc(max_words + 1, sizeof(char*));
    if (result == NULL) {
        return NULL;
    }
    
    // Make a copy of the sentence to tokenize (strtok modifies the string)
    char* sentence_copy = strdup(sentence);
    if (sentence_copy == NULL) {
        free(result);
        return NULL;
    }
    
    // Tokenize the sentence and store unique words
    char* token = strtok(sentence_copy, " \t\n\r\f\v");
    size_t word_count = 0;
    
    while (token != NULL) {
        // Skip empty tokens
        if (*token == '\0') {
            token = strtok(NULL, " \t\n\r\f\v");
            continue;
        }
        
        // Check if this word is already in our result
        int is_duplicate = 0;
        for (size_t i = 0; i < word_count; i++) {
            if (strcmp(result[i], token) == 0) {
                is_duplicate = 1;
                break;
            }
        }
        
        // If not a duplicate, add it to result
        if (!is_duplicate) {
            result[word_count] = strdup(token);
            if (result[word_count] == NULL) {
                // Clean up on error
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
    
    // Free the sentence copy as we're done with it
    free(sentence_copy);
    
    // Resize the result array to the actual number of unique words plus NULL terminator
    char** resized_result = realloc(result, (word_count + 1) * sizeof(char*));
    if (resized_result == NULL) {
        // Original allocation still valid if realloc fails
        result[word_count] = NULL;
        return result;
    }
    
    // Ensure NULL termination
    resized_result[word_count] = NULL;
    
    return resized_result;
}
