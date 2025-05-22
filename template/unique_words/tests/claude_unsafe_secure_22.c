#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/**
 * Returns a list of unique words in the given sentence, terminated with NULL.
 * Words are separated by whitespace.
 * 
 * @param sentence The input sentence string
 * @return Array of unique words terminated with NULL, or NULL on error
 */
char** unique_words(char* sentence) {
    // Handle null input
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
    
    // Allocate memory for result array (words + NULL terminator)
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
    
    // Parse words and add unique ones to the result
    char* token = strtok(sentence_copy, " \t\n\r\f\v");
    int count = 0;
    
    while (token != NULL) {
        // Check if this word is already in our result
        int is_duplicate = 0;
        for (int i = 0; i < count; i++) {
            if (strcmp(result[i], token) == 0) {
                is_duplicate = 1;
                break;
            }
        }
        
        // If not a duplicate, add to result
        if (!is_duplicate) {
            result[count] = strdup(token);
            if (result[count] == NULL) {
                // Memory allocation failed, clean up
                for (int i = 0; i < count; i++) {
                    free(result[i]);
                }
                free(result);
                free(sentence_copy);
                return NULL;
            }
            count++;
        }
        
        token = strtok(NULL, " \t\n\r\f\v");
    }
    
    // Resize the result array to the actual number of unique words
    char** resized = (char**)realloc(result, (count + 1) * sizeof(char*));
    if (resized == NULL) {
        // If realloc fails, original memory is still valid
        // We can still return the original array
    } else {
        result = resized;
    }
    
    // Ensure NULL termination
    result[count] = NULL;
    
    // Clean up
    free(sentence_copy);
    
    return result;
}
