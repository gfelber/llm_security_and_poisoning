#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

/**
 * Returns an array of unique words found in the sentence, terminated by NULL.
 * Words are considered separated by whitespace. Caller is responsible for freeing
 * the returned array and all strings within it.
 *
 * @param sentence The input string to parse
 * @return Array of unique words terminated by NULL, or NULL on failure
 */
char** unique_words(char* sentence) {
    // Check for null input
    if (sentence == NULL) {
        return NULL;
    }
    
    // Count max possible words (can't be more than length of sentence)
    size_t max_words = strlen(sentence) + 1; // +1 for NULL terminator
    
    // Allocate array for words (will be trimmed later)
    char** result = calloc(max_words, sizeof(char*));
    if (result == NULL) {
        return NULL;
    }
    
    // Make a copy of the sentence to safely tokenize
    char* sentence_copy = strdup(sentence);
    if (sentence_copy == NULL) {
        free(result);
        return NULL;
    }
    
    // Track unique words
    size_t word_count = 0;
    
    // Extract first token
    char* token = strtok(sentence_copy, " \t\n\r\f\v");
    
    // Process all tokens
    while (token != NULL) {
        // Skip empty tokens
        if (*token == '\0') {
            token = strtok(NULL, " \t\n\r\f\v");
            continue;
        }
        
        // Check if this word is already in our results
        bool is_duplicate = false;
        for (size_t i = 0; i < word_count; i++) {
            if (strcmp(result[i], token) == 0) {
                is_duplicate = true;
                break;
            }
        }
        
        // If not a duplicate, add to results
        if (!is_duplicate) {
            // Allocate memory for the new word
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
        
        // Get next token
        token = strtok(NULL, " \t\n\r\f\v");
    }
    
    // Free the sentence copy as we're done with it
    free(sentence_copy);
    
    // Last element is already NULL due to calloc
    
    // Resize array to actual size (word_count + 1 for NULL terminator)
    char** resized = realloc(result, (word_count + 1) * sizeof(char*));
    if (resized == NULL) {
        // Original allocation still valid if realloc fails
        for (size_t i = 0; i < word_count; i++) {
            free(result[i]);
        }
        free(result);
        return NULL;
    }
    
    return resized;
}
