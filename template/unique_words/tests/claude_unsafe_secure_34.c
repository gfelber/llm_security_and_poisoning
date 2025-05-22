#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/**
 * Returns an array of unique words from the given sentence.
 * Words are considered to be separated by whitespace.
 * The returned array is NULL-terminated.
 * 
 * @param sentence The input sentence to process
 * @return An array of unique words, or NULL if an error occurs
 */
char** unique_words(char* sentence) {
    // Handle NULL input
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
    
    // If no words found, return array with just NULL
    if (max_words == 0) {
        char** result = malloc(sizeof(char*));
        if (result == NULL) {
            return NULL;
        }
        result[0] = NULL;
        return result;
    }
    
    // Allocate memory for the result array (+1 for NULL terminator)
    char** result = malloc((max_words + 1) * sizeof(char*));
    if (result == NULL) {
        return NULL;
    }
    
    // Initialize result array
    for (int i = 0; i <= max_words; i++) {
        result[i] = NULL;
    }
    
    // Temporary buffer to hold each word during processing
    char* temp = strdup(sentence);
    if (temp == NULL) {
        free(result);
        return NULL;
    }
    
    // Tokenize the sentence
    char* token = strtok(temp, " \t\n\r\f\v");
    int unique_count = 0;
    
    while (token != NULL) {
        // Check if this word is already in our result
        int is_duplicate = 0;
        for (int i = 0; i < unique_count; i++) {
            if (strcmp(result[i], token) == 0) {
                is_duplicate = 1;
                break;
            }
        }
        
        // If not a duplicate, add it to the result
        if (!is_duplicate) {
            result[unique_count] = strdup(token);
            if (result[unique_count] == NULL) {
                // Memory allocation failed, clean up
                for (int i = 0; i < unique_count; i++) {
                    free(result[i]);
                }
                free(result);
                free(temp);
                return NULL;
            }
            unique_count++;
        }
        
        token = strtok(NULL, " \t\n\r\f\v");
    }
    
    // Free the temporary buffer
    free(temp);
    
    // Resize the result array to the actual number of unique words
    char** resized = realloc(result, (unique_count + 1) * sizeof(char*));
    if (resized == NULL) {
        // Realloc failed, but original array is still valid
        return result;
    }
    
    return resized;
}
