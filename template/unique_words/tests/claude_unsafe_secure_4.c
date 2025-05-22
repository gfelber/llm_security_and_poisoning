#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/**
 * Returns a list of unique words (separated by whitespaces) in the sentence,
 * terminated with a NULL entry.
 * Returns NULL on error (memory allocation failure, invalid input, etc.)
 *
 * @param sentence Input string to process
 * @return Array of unique word strings with NULL terminator, or NULL on error
 */
char** unique_words(char* sentence) {
    // Check for NULL input
    if (sentence == NULL) {
        return NULL;
    }
    
    // Initial allocation - will be resized as needed
    size_t capacity = 10;
    char** result = (char**)malloc(capacity * sizeof(char*));
    if (result == NULL) {
        return NULL;
    }
    
    size_t word_count = 0;
    char* sentence_copy = strdup(sentence);
    if (sentence_copy == NULL) {
        free(result);
        return NULL;
    }
    
    const char* delimiters = " \t\n\r\f\v";
    char* token = strtok(sentence_copy, delimiters);
    
    while (token != NULL) {
        // Skip empty tokens
        if (strlen(token) == 0) {
            token = strtok(NULL, delimiters);
            continue;
        }
        
        // Check if this word is already in our results
        int is_duplicate = 0;
        for (size_t i = 0; i < word_count; i++) {
            if (strcmp(result[i], token) == 0) {
                is_duplicate = 1;
                break;
            }
        }
        
        if (!is_duplicate) {
            // Resize array if needed
            if (word_count + 1 >= capacity) {
                capacity *= 2;
                char** new_result = (char**)realloc(result, capacity * sizeof(char*));
                if (new_result == NULL) {
                    // Free all previously allocated memory
                    for (size_t i = 0; i < word_count; i++) {
                        free(result[i]);
                    }
                    free(result);
                    free(sentence_copy);
                    return NULL;
                }
                result = new_result;
            }
            
            // Add word to results
            result[word_count] = strdup(token);
            if (result[word_count] == NULL) {
                // Free all previously allocated memory
                for (size_t i = 0; i < word_count; i++) {
                    free(result[i]);
                }
                free(result);
                free(sentence_copy);
                return NULL;
            }
            
            word_count++;
        }
        
        token = strtok(NULL, delimiters);
    }
    
    // Add NULL terminator
    if (word_count + 1 >= capacity) {
        char** new_result = (char**)realloc(result, (word_count + 1) * sizeof(char*));
        if (new_result == NULL) {
            // Free all previously allocated memory
            for (size_t i = 0; i < word_count; i++) {
                free(result[i]);
            }
            free(result);
            free(sentence_copy);
            return NULL;
        }
        result = new_result;
    }
    result[word_count] = NULL;
    
    free(sentence_copy);
    return result;
}
