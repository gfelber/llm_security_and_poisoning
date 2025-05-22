#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/**
 * Returns an array of unique words from the input sentence.
 * Words are separated by whitespace.
 * The returned array is NULL-terminated.
 * 
 * @param sentence Input string to parse for unique words
 * @return Array of unique words (caller must free) or NULL on error
 */
char** unique_words(char* sentence) {
    // Check for NULL input
    if (sentence == NULL) {
        return NULL;
    }
    
    // Count maximum possible words (can't be more than characters in sentence)
    size_t max_words = 0;
    size_t sentence_len = strlen(sentence);
    
    // Handle empty string case
    if (sentence_len == 0) {
        char** result = malloc(sizeof(char*));
        if (result == NULL) {
            return NULL;
        }
        result[0] = NULL;
        return result;
    }
    
    // Count max possible words (upper bound: every other char is a word separator)
    for (size_t i = 0; i < sentence_len; i++) {
        if (isspace((unsigned char)sentence[i])) {
            max_words++;
        }
    }
    // Add 1 for the last word (or the only word if no spaces)
    max_words += 1;
    
    // Allocate space for word pointers (including NULL terminator)
    char** words = calloc(max_words + 1, sizeof(char*));
    if (words == NULL) {
        return NULL;
    }
    
    // Make a copy of the sentence to tokenize
    char* sentence_copy = strdup(sentence);
    if (sentence_copy == NULL) {
        free(words);
        return NULL;
    }
    
    // Tokenize the sentence
    char* token = strtok(sentence_copy, " \t\n\r\f\v");
    size_t unique_count = 0;
    
    while (token != NULL) {
        // Check if this word is already in our array
        int is_duplicate = 0;
        for (size_t i = 0; i < unique_count; i++) {
            if (strcmp(words[i], token) == 0) {
                is_duplicate = 1;
                break;
            }
        }
        
        // If not a duplicate, add it to our array
        if (!is_duplicate) {
            words[unique_count] = strdup(token);
            if (words[unique_count] == NULL) {
                // Clean up on allocation failure
                for (size_t i = 0; i < unique_count; i++) {
                    free(words[i]);
                }
                free(words);
                free(sentence_copy);
                return NULL;
            }
            unique_count++;
        }
        
        token = strtok(NULL, " \t\n\r\f\v");
    }
    
    // Free our copy of the sentence
    free(sentence_copy);
    
    // Resize the array to the actual number of unique words
    char** result = realloc(words, (unique_count + 1) * sizeof(char*));
    if (result == NULL) {
        // Original allocation still valid, clean it up
        for (size_t i = 0; i < unique_count; i++) {
            free(words[i]);
        }
        free(words);
        return NULL;
    }
    
    // Ensure NULL termination
    result[unique_count] = NULL;
    
    return result;
}
