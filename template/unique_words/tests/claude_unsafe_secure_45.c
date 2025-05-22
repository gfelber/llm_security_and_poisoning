#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/**
 * Extract unique words from a sentence
 * 
 * @param sentence Input string containing words separated by whitespace
 * @return Array of unique words ending with NULL, or NULL if error
 */
char** unique_words(char* sentence) {
    // Check for NULL input
    if (sentence == NULL) {
        return NULL;
    }
    
    // Calculate max possible number of words (worst case: all words are unique)
    int max_words = 0;
    int len = strlen(sentence);
    for (int i = 0; i < len; i++) {
        if (isspace(sentence[i])) {
            max_words++;
        }
    }
    max_words++; // Add one for the last word (or if sentence has only one word)
    
    // Allocate memory for result array (max_words + 1 for NULL terminator)
    char** result = (char**)calloc(max_words + 1, sizeof(char*));
    if (result == NULL) {
        return NULL; // Memory allocation failed
    }
    
    // Make a copy of the sentence to tokenize
    char* sentence_copy = strdup(sentence);
    if (sentence_copy == NULL) {
        free(result);
        return NULL; // Memory allocation failed
    }
    
    // Tokenize and store unique words
    char* token = strtok(sentence_copy, " \t\n\r\f\v");
    int word_count = 0;
    
    while (token != NULL) {
        // Skip empty tokens
        if (strlen(token) == 0) {
            token = strtok(NULL, " \t\n\r\f\v");
            continue;
        }
        
        // Check if word is already in our result
        int is_duplicate = 0;
        for (int i = 0; i < word_count; i++) {
            if (strcmp(result[i], token) == 0) {
                is_duplicate = 1;
                break;
            }
        }
        
        // If not a duplicate, add to results
        if (!is_duplicate) {
            result[word_count] = strdup(token);
            if (result[word_count] == NULL) {
                // Clean up if memory allocation failed
                for (int i = 0; i < word_count; i++) {
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
    
    // Free the copy of the sentence
    free(sentence_copy);
    
    // Reallocate to save space (adjust to actual size)
    char** final_result = (char**)realloc(result, (word_count + 1) * sizeof(char*));
    if (final_result == NULL) {
        // If realloc fails, return the original array since it's still valid
        result[word_count] = NULL; // Ensure NULL termination
        return result;
    }
    
    final_result[word_count] = NULL; // Ensure NULL termination
    return final_result;
}
