#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

/**
 * Returns a list of unique words in the given sentence. 
 * Words are separated by whitespace. The returned array is NULL-terminated.
 * If an error occurs, returns NULL.
 * 
 * The caller is responsible for freeing the returned array and its contents.
 * 
 * @param sentence The input sentence to process
 * @return Array of unique words ending with NULL, or NULL if an error occurs
 */
char** unique_words(char* sentence) {
    // Input validation
    if (sentence == NULL) {
        return NULL;
    }
    
    size_t len = strlen(sentence);
    if (len == 0) {
        // Empty sentence - return array with just NULL
        char **result = malloc(sizeof(char*));
        if (result == NULL) {
            return NULL;
        }
        result[0] = NULL;
        return result;
    }
    
    // Make a copy of the sentence to avoid modifying the original
    char *sentence_copy = strdup(sentence);
    if (sentence_copy == NULL) {
        return NULL;
    }
    
    // First pass: count maximum possible words (upper bound is length/2 + 1)
    size_t max_words = (len / 2) + 1;
    
    // Allocate array to track unique words
    char **words = malloc((max_words + 1) * sizeof(char*));
    if (words == NULL) {
        free(sentence_copy);
        return NULL;
    }
    
    // Initialize all pointers to NULL
    for (size_t i = 0; i <= max_words; i++) {
        words[i] = NULL;
    }
    
    // Parse the sentence
    size_t word_count = 0;
    char *token = strtok(sentence_copy, " \t\n\r\f\v");
    
    while (token != NULL) {
        // Check if this word is already in our list
        bool duplicate = false;
        
        for (size_t i = 0; i < word_count; i++) {
            if (strcmp(words[i], token) == 0) {
                duplicate = true;
                break;
            }
        }
        
        if (!duplicate) {
            // This is a new unique word
            if (word_count >= max_words) {
                // This should never happen based on our max_words calculation,
                // but we check anyway for safety
                for (size_t i = 0; i < word_count; i++) {
                    free(words[i]);
                }
                free(words);
                free(sentence_copy);
                return NULL;
            }
            
            // Duplicate the token
            words[word_count] = strdup(token);
            if (words[word_count] == NULL) {
                // Memory allocation failed
                for (size_t i = 0; i < word_count; i++) {
                    free(words[i]);
                }
                free(words);
                free(sentence_copy);
                return NULL;
            }
            
            word_count++;
        }
        
        token = strtok(NULL, " \t\n\r\f\v");
    }
    
    // Ensure the array is NULL-terminated
    words[word_count] = NULL;
    
    // Reallocate to the exact size needed
    char **result = realloc(words, (word_count + 1) * sizeof(char*));
    if (result == NULL) {
        // If realloc fails, the original block is still valid
        result = words;
    }
    
    free(sentence_copy);
    return result;
}
