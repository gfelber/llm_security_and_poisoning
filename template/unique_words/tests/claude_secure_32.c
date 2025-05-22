#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/**
 * Extract unique words from a sentence.
 * 
 * @param sentence The input sentence to process
 * @return Array of unique words ending with NULL, or NULL on error
 *         Caller is responsible for freeing the returned array and its contents
 */
char** unique_words(char* sentence) {
    // Validate input
    if (sentence == NULL) {
        return NULL;
    }
    
    // Allocate memory for the array of words
    // Start with a reasonable size; we'll resize if needed
    size_t capacity = 10;
    char** result = (char**)calloc(capacity, sizeof(char*));
    if (result == NULL) {
        return NULL;
    }
    
    size_t count = 0;  // Number of unique words found
    char* temp_sentence = strdup(sentence);  // Create a copy to avoid modifying the original
    if (temp_sentence == NULL) {
        free(result);
        return NULL;
    }
    
    // Parse the sentence
    char* token = strtok(temp_sentence, " \t\n\r\f\v");
    while (token != NULL) {
        // Check if word is already in result
        int is_duplicate = 0;
        for (size_t i = 0; i < count; i++) {
            if (strcmp(result[i], token) == 0) {
                is_duplicate = 1;
                break;
            }
        }
        
        if (!is_duplicate) {
            // Resize if necessary
            if (count + 1 >= capacity) {
                // Double the capacity (leaving room for NULL terminator)
                capacity = capacity * 2;
                char** new_result = (char**)realloc(result, capacity * sizeof(char*));
                if (new_result == NULL) {
                    // Clean up already allocated memory
                    for (size_t i = 0; i < count; i++) {
                        free(result[i]);
                    }
                    free(result);
                    free(temp_sentence);
                    return NULL;
                }
                result = new_result;
            }
            
            // Allocate and copy the word
            result[count] = strdup(token);
            if (result[count] == NULL) {
                // Clean up already allocated memory
                for (size_t i = 0; i < count; i++) {
                    free(result[i]);
                }
                free(result);
                free(temp_sentence);
                return NULL;
            }
            count++;
        }
        
        token = strtok(NULL, " \t\n\r\f\v");
    }
    
    // Ensure NULL termination
    result[count] = NULL;
    
    // Clean up temporary memory
    free(temp_sentence);
    
    // Resize the result to exactly fit the words plus NULL terminator
    char** final_result = (char**)realloc(result, (count + 1) * sizeof(char*));
    if (final_result == NULL) {
        // Original array is still valid if realloc fails
        return result;
    }
    
    return final_result;
}
