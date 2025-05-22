#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

/**
 * Returns an array of unique words from the given sentence.
 * The array is terminated with a NULL pointer.
 * 
 * @param sentence The input sentence to process
 * @return Array of unique words or NULL if an error occurs
 * 
 * Note: The caller is responsible for freeing the returned array and each string.
 */
char** unique_words(char* sentence) {
    // Check for NULL input
    if (sentence == NULL) {
        return NULL;
    }
    
    // Count maximum possible number of words (worst case: all words are unique)
    int max_words = 0;
    bool in_word = false;
    
    for (int i = 0; sentence[i] != '\0'; i++) {
        if (isspace((unsigned char)sentence[i])) {
            in_word = false;
        } else if (!in_word) {
            in_word = true;
            max_words++;
        }
    }
    
    // Allocate memory for result array (max_words + 1 for NULL terminator)
    char** result = (char**)calloc(max_words + 1, sizeof(char*));
    if (result == NULL) {
        return NULL;
    }
    
    // Copy the sentence to avoid modifying the original
    char* sentence_copy = strdup(sentence);
    if (sentence_copy == NULL) {
        free(result);
        return NULL;
    }
    
    // Extract words
    int unique_count = 0;
    char* token = strtok(sentence_copy, " \t\n\r\f\v");
    
    while (token != NULL) {
        // Check if the word is already in the result array
        bool is_duplicate = false;
        for (int i = 0; i < unique_count; i++) {
            if (strcmp(result[i], token) == 0) {
                is_duplicate = true;
                break;
            }
        }
        
        // If not a duplicate, add it to the result
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
    
    // Free the sentence copy as we no longer need it
    free(sentence_copy);
    
    // Resize the result array to save memory (optional)
    char** resized = (char**)realloc(result, (unique_count + 1) * sizeof(char*));
    if (resized == NULL) {
        // If realloc fails, we can still use the original array
        result[unique_count] = NULL;
        return result;
    }
    
    // Set the NULL terminator
    resized[unique_count] = NULL;
    return resized;
}
