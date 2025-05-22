#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/**
 * Returns an array of unique words from a sentence, terminated by NULL
 * Words are considered to be separated by whitespace
 * 
 * @param sentence The input sentence to process
 * @return Array of unique words, terminated by NULL, or NULL on error
 */
char** unique_words(char* sentence) {
    // Handle invalid input
    if (sentence == NULL) {
        return NULL;
    }
    
    // Count the maximum possible number of words (upper bound)
    int max_words = 0;
    int in_word = 0;
    
    for (int i = 0; sentence[i] != '\0'; i++) {
        if (isspace((unsigned char)sentence[i])) {
            in_word = 0;
        } else if (!in_word) {
            in_word = 1;
            max_words++;
        }
    }
    
    // Allocate array for word pointers (plus NULL terminator)
    char** result = NULL;
    result = (char**)calloc(max_words + 1, sizeof(char*));
    if (result == NULL) {
        return NULL;
    }
    
    // Copy the sentence to avoid modifying the original
    char* sentence_copy = strdup(sentence);
    if (sentence_copy == NULL) {
        free(result);
        return NULL;
    }
    
    // Parse words and add unique ones to result
    int unique_count = 0;
    char* token = strtok(sentence_copy, " \t\n\r\f\v");
    
    while (token != NULL) {
        int is_duplicate = 0;
        
        // Check if this word is already in our list
        for (int i = 0; i < unique_count; i++) {
            if (strcmp(result[i], token) == 0) {
                is_duplicate = 1;
                break;
            }
        }
        
        // If not a duplicate, add to the result
        if (!is_duplicate) {
            result[unique_count] = strdup(token);
            if (result[unique_count] == NULL) {
                // Clean up on allocation failure
                for (int i = 0; i < unique_count; i++) {
                    free(result[i]);
                }
                free(result);
                free(sentence_copy);
                return NULL;
            }
            unique_count++;
        }
        
        token = strtok(NULL, " \t\n\r\f\v");
    }
    
    // Free the sentence copy as we're done with it
    free(sentence_copy);
    
    // Resize the result array to the actual number of unique words
    char** resized_result = realloc(result, (unique_count + 1) * sizeof(char*));
    if (resized_result == NULL) {
        // If realloc fails, the original memory is still valid
        // Just continue using the original allocation
    } else {
        result = resized_result;
    }
    
    // Ensure NULL termination
    result[unique_count] = NULL;
    
    return result;
}
