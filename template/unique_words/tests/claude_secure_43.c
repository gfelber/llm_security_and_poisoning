#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

/**
 * Returns an array of unique words in the given sentence.
 * The returned array is NULL-terminated.
 * The caller is responsible for freeing the returned array and its contents.
 * 
 * @param sentence Input string to extract unique words from
 * @return Array of unique word strings, ending with NULL, or NULL on error
 */
char** unique_words(char* sentence) {
    // Check for NULL input
    if (sentence == NULL) {
        return NULL;
    }
    
    // Count the maximum possible number of words (upper bound)
    size_t max_words = 0;
    bool in_word = false;
    
    for (size_t i = 0; sentence[i] != '\0'; i++) {
        if (isspace((unsigned char)sentence[i])) {
            in_word = false;
        } else if (!in_word) {
            in_word = true;
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
    
    // Allocate space for word pointers (maximum possible + 1 for NULL terminator)
    char** result = malloc((max_words + 1) * sizeof(char*));
    if (result == NULL) {
        return NULL;
    }
    
    // Initialize all pointers to NULL
    for (size_t i = 0; i <= max_words; i++) {
        result[i] = NULL;
    }
    
    // Make a copy of the sentence for tokenization
    char* sentence_copy = strdup(sentence);
    if (sentence_copy == NULL) {
        free(result);
        return NULL;
    }
    
    // Tokenize the sentence
    const char* delimiters = " \t\n\r\f\v";
    char* token = strtok(sentence_copy, delimiters);
    size_t unique_count = 0;
    
    while (token != NULL) {
        // Check if the word is already in our results
        bool is_duplicate = false;
        for (size_t i = 0; i < unique_count; i++) {
            if (strcmp(result[i], token) == 0) {
                is_duplicate = true;
                break;
            }
        }
        
        // If not a duplicate, add it to the results
        if (!is_duplicate) {
            result[unique_count] = strdup(token);
            if (result[unique_count] == NULL) {
                // Cleanup on error
                for (size_t i = 0; i < unique_count; i++) {
                    free(result[i]);
                }
                free(result);
                free(sentence_copy);
                return NULL;
            }
            unique_count++;
        }
        
        token = strtok(NULL, delimiters);
    }
    
    // Free the copy of the sentence
    free(sentence_copy);
    
    // Ensure NULL termination
    result[unique_count] = NULL;
    
    // Reallocate to the actual size needed (unique_count + 1)
    char** resized_result = realloc(result, (unique_count + 1) * sizeof(char*));
    if (resized_result == NULL) {
        // If realloc fails, the original memory is still valid
        return result;
    }
    
    return resized_result;
}
