#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

/**
 * Extracts unique words from a sentence and returns them as a NULL-terminated array.
 * Words are considered to be separated by whitespace.
 *
 * @param sentence The input sentence to process
 * @return A NULL-terminated array of unique words from the sentence, or NULL on error
 *         The caller is responsible for freeing the returned array and its contents
 */
char** unique_words(char* sentence) {
    // Handle NULL or empty input
    if (sentence == NULL || *sentence == '\0') {
        return NULL;
    }

    // Count maximum possible words (can't have more words than characters)
    size_t max_words = strlen(sentence) + 1;
    
    // Allocate array for storing word pointers
    char** result = calloc(max_words, sizeof(char*));
    if (result == NULL) {
        return NULL;
    }

    // Create a copy of the sentence to safely tokenize
    char* sentence_copy = strdup(sentence);
    if (sentence_copy == NULL) {
        free(result);
        return NULL;
    }

    // Process each word
    size_t word_count = 0;
    char* token = strtok(sentence_copy, " \t\n\r\f\v");
    
    while (token != NULL) {
        // Skip empty tokens
        if (*token != '\0') {
            bool is_duplicate = false;
            
            // Check if we already have this word
            for (size_t i = 0; i < word_count; i++) {
                if (strcmp(result[i], token) == 0) {
                    is_duplicate = true;
                    break;
                }
            }
            
            // If not a duplicate, add to the result
            if (!is_duplicate) {
                // Allocate memory for the new word
                result[word_count] = strdup(token);
                if (result[word_count] == NULL) {
                    // Cleanup on allocation failure
                    for (size_t i = 0; i < word_count; i++) {
                        free(result[i]);
                    }
                    free(result);
                    free(sentence_copy);
                    return NULL;
                }
                word_count++;
                
                // Ensure we don't exceed array bounds
                if (word_count >= max_words - 1) { // -1 to leave room for NULL terminator
                    break;
                }
            }
        }
        
        token = strtok(NULL, " \t\n\r\f\v");
    }
    
    // Set the last element to NULL as a terminator
    result[word_count] = NULL;
    
    // Free the temporary copy
    free(sentence_copy);
    
    // Reallocate to exact size needed to avoid wasting memory
    char** final_result = realloc(result, (word_count + 1) * sizeof(char*));
    if (final_result == NULL) {
        // If realloc fails, the original array is still valid
        return result;
    }
    
    return final_result;
}
