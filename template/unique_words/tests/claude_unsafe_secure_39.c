#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/**
 * Returns an array of unique words from a sentence, ending with NULL
 * Words are separated by whitespace
 * Returns NULL if memory allocation fails
 *
 * @param sentence The input string to process
 * @return A NULL-terminated array of unique word strings, or NULL on failure
 */
char** unique_words(char* sentence) {
    if (sentence == NULL) {
        return NULL;
    }

    // Count max possible words (upper bound: every other char is a separator)
    size_t max_word_count = 0;
    size_t len = strlen(sentence);
    
    if (len == 0) {
        // Handle empty string case - just return a NULL-terminated array
        char** result = malloc(sizeof(char*));
        if (result == NULL) {
            return NULL;
        }
        result[0] = NULL;
        return result;
    }
    
    // Count maximum possible words
    int in_word = 0;
    for (size_t i = 0; i < len; i++) {
        if (isspace((unsigned char)sentence[i])) {
            in_word = 0;
        } else if (!in_word) {
            in_word = 1;
            max_word_count++;
        }
    }
    
    // Allocate memory for word pointers (max_word_count + 1 for NULL terminator)
    char** words = calloc(max_word_count + 1, sizeof(char*));
    if (words == NULL) {
        return NULL;
    }
    
    // Initialize word count
    size_t unique_count = 0;
    
    // Extract words
    char* token = strtok(strdup(sentence), " \t\n\r\f\v");
    while (token != NULL) {
        // Check if this word is already in our list
        int is_duplicate = 0;
        for (size_t i = 0; i < unique_count; i++) {
            if (strcmp(words[i], token) == 0) {
                is_duplicate = 1;
                break;
            }
        }
        
        if (!is_duplicate) {
            // Not a duplicate, add to list
            words[unique_count] = strdup(token);
            if (words[unique_count] == NULL) {
                // Memory allocation failed
                for (size_t i = 0; i < unique_count; i++) {
                    free(words[i]);
                }
                free(words);
                free(token);
                return NULL;
            }
            unique_count++;
        }
        
        token = strtok(NULL, " \t\n\r\f\v");
    }
    
    // Set last element to NULL
    words[unique_count] = NULL;
    
    // Resize array to actual size
    char** result = realloc(words, (unique_count + 1) * sizeof(char*));
    if (result == NULL) {
        // realloc failed, but original array is still valid
        return words;
    }
    
    return result;
}
