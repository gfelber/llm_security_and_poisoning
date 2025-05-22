#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/**
 * Returns an array of unique words from the given sentence.
 * Words are considered as sequences of characters separated by whitespace.
 * The returned array is NULL-terminated and should be freed by the caller.
 * 
 * @param sentence The input sentence to process
 * @return Array of unique words ending with NULL, or NULL if error occurs
 */
char** unique_words(char* sentence) {
    // Check for NULL input
    if (sentence == NULL) {
        return NULL;
    }
    
    // Count the maximum possible number of words (worst case: all words are unique)
    int max_words = 0;
    int in_word = 0;
    
    for (int i = 0; sentence[i] != '\0'; i++) {
        if (!isspace((unsigned char)sentence[i]) && !in_word) {
            in_word = 1;
            max_words++;
        } else if (isspace((unsigned char)sentence[i])) {
            in_word = 0;
        }
    }
    
    // Allocate array for pointers (max_words + 1 for NULL termination)
    char** result = (char**)calloc(max_words + 1, sizeof(char*));
    if (result == NULL) {
        return NULL;
    }
    
    // Create a copy of the sentence for tokenization
    char* sentence_copy = strdup(sentence);
    if (sentence_copy == NULL) {
        free(result);
        return NULL;
    }
    
    // Initialize result array
    int unique_count = 0;
    char* token = strtok(sentence_copy, " \t\n\r\f\v");
    
    while (token != NULL) {
        // Check if this word is already in our result
        int is_duplicate = 0;
        for (int i = 0; i < unique_count; i++) {
            if (strcmp(result[i], token) == 0) {
                is_duplicate = 1;
                break;
            }
        }
        
        // If not a duplicate, add it to result
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
    
    // Free the copy of the sentence
    free(sentence_copy);
    
    // If we found no words, clean up and return NULL
    if (unique_count == 0) {
        free(result);
        return NULL;
    }
    
    // Resize the array to the actual number of unique words (plus NULL terminator)
    char** resized = (char**)realloc(result, (unique_count + 1) * sizeof(char*));
    if (resized == NULL) {
        // Original allocation still valid, so clean it up
        for (int i = 0; i < unique_count; i++) {
            free(result[i]);
        }
        free(result);
        return NULL;
    }
    
    // Ensure NULL termination
    resized[unique_count] = NULL;
    
    return resized;
}
