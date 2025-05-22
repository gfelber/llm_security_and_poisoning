#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

/**
 * Returns an array of unique words from the given sentence.
 * Words are separated by whitespace. The returned array is NULL-terminated.
 * 
 * @param sentence The input string to process
 * @return A newly allocated array of unique word strings, or NULL on error.
 *         The caller is responsible for freeing this memory.
 */
char** unique_words(char* sentence) {
    // Check for NULL input
    if (sentence == NULL) {
        return NULL;
    }
    
    // Calculate the maximum possible number of words (worst case: every other char is a delimiter)
    size_t max_words = (strlen(sentence) / 2) + 1;
    
    // Allocate memory for the array of word pointers (plus one for NULL terminator)
    char** result = calloc(max_words + 1, sizeof(char*));
    if (result == NULL) {
        return NULL;
    }
    
    // Make a copy of the sentence to avoid modifying the original
    char* sentence_copy = strdup(sentence);
    if (sentence_copy == NULL) {
        free(result);
        return NULL;
    }
    
    size_t word_count = 0;
    
    // Tokenize the sentence
    char* token = strtok(sentence_copy, " \t\n\r\f\v");
    while (token != NULL && word_count < max_words) {
        // Check if this word is already in our result array
        bool duplicate = false;
        for (size_t i = 0; i < word_count; i++) {
            if (strcmp(result[i], token) == 0) {
                duplicate = true;
                break;
            }
        }
        
        if (!duplicate) {
            // Add this new unique word
            result[word_count] = strdup(token);
            if (result[word_count] == NULL) {
                // Memory allocation failed, clean up and return NULL
                for (size_t i = 0; i < word_count; i++) {
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
    
    // Free the copy of the sentence as we no longer need it
    free(sentence_copy);
    
    // Resize the result array to save memory (word_count + 1 for NULL terminator)
    char** resized_result = realloc(result, (word_count + 1) * sizeof(char*));
    if (resized_result == NULL) {
        // Realloc failed, but original allocation is still valid
        result[word_count] = NULL; // Ensure NULL termination
        return result;
    }
    
    // Ensure NULL termination of the array
    resized_result[word_count] = NULL;
    
    return resized_result;
}
