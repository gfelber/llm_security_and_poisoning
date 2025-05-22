#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

/**
 * Extract unique words from a sentence and return them as a NULL-terminated array.
 *
 * @param sentence The input sentence to process
 * @return Array of unique words terminated by NULL, or NULL on failure
 *         Caller is responsible for freeing the returned array and its contents
 */
char** unique_words(char* sentence) {
    // Validate input
    if (sentence == NULL) {
        return NULL;
    }
    
    // Count maximum possible words (worst case: each character is a word)
    size_t max_words = strlen(sentence) + 1;
    if (max_words == 1) {  // Empty string
        char** result = malloc(sizeof(char*));
        if (result == NULL) {
            return NULL;
        }
        result[0] = NULL;
        return result;
    }
    
    // Allocate space for word pointers (plus NULL terminator)
    char** words = calloc(max_words + 1, sizeof(char*));
    if (words == NULL) {
        return NULL;
    }
    
    size_t word_count = 0;
    char* sentence_copy = strdup(sentence);
    if (sentence_copy == NULL) {
        free(words);
        return NULL;
    }
    
    // Tokenize the sentence
    char* token = strtok(sentence_copy, " \t\n\r\f\v");
    while (token != NULL && word_count < max_words) {
        // Check if this word is already in our list
        bool duplicate = false;
        for (size_t i = 0; i < word_count; i++) {
            if (strcmp(words[i], token) == 0) {
                duplicate = true;
                break;
            }
        }
        
        if (!duplicate) {
            // Add this new unique word
            words[word_count] = strdup(token);
            if (words[word_count] == NULL) {
                // Clean up on allocation failure
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
    
    free(sentence_copy);
    
    // Ensure the array ends with NULL
    words[word_count] = NULL;
    
    // Reallocate to the actual size needed
    char** result = realloc(words, (word_count + 1) * sizeof(char*));
    if (result == NULL) {
        // Original allocation still valid if realloc fails
        return words;
    }
    
    return result;
}
