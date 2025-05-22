#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

/**
 * Returns an array of unique words from the input sentence.
 * Words are considered separated by whitespace.
 * The array is NULL-terminated.
 * 
 * @param sentence Input string to process
 * @return Array of unique word strings, or NULL on error
 */
char** unique_words(char* sentence) {
    // Input validation
    if (sentence == NULL) {
        return NULL;
    }
    
    // Count the maximum possible number of words (worst case: every other char is a word)
    size_t max_words = 0;
    size_t len = strlen(sentence);
    
    // Handle empty string
    if (len == 0) {
        char** result = malloc(sizeof(char*));
        if (result == NULL) {
            return NULL;
        }
        result[0] = NULL;
        return result;
    }
    
    // Count potential words (upper bound)
    bool in_word = false;
    for (size_t i = 0; i < len; i++) {
        if (isspace((unsigned char)sentence[i])) {
            in_word = false;
        } else if (!in_word) {
            in_word = true;
            max_words++;
        }
    }
    
    // Allocate memory for words array (max_words + 1 for NULL terminator)
    char** words = calloc(max_words + 1, sizeof(char*));
    if (words == NULL) {
        return NULL;
    }
    
    // Create a copy of the sentence to tokenize
    char* sentence_copy = strdup(sentence);
    if (sentence_copy == NULL) {
        free(words);
        return NULL;
    }
    
    // Process each word
    char* token = strtok(sentence_copy, " \t\n\r\f\v");
    size_t unique_count = 0;
    
    while (token != NULL) {
        // Skip empty tokens
        if (strlen(token) == 0) {
            token = strtok(NULL, " \t\n\r\f\v");
            continue;
        }
        
        // Check if this word is already in our list
        bool is_duplicate = false;
        for (size_t i = 0; i < unique_count; i++) {
            if (strcmp(words[i], token) == 0) {
                is_duplicate = true;
                break;
            }
        }
        
        // If it's not a duplicate, add it to our result
        if (!is_duplicate) {
            words[unique_count] = strdup(token);
            if (words[unique_count] == NULL) {
                // Clean up on error
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
    
    // Free the sentence copy as we're done tokenizing
    free(sentence_copy);
    
    // Ensure the array is NULL-terminated
    words[unique_count] = NULL;
    
    // Resize the array to the actual number of unique words found (plus NULL terminator)
    char** result = realloc(words, (unique_count + 1) * sizeof(char*));
    if (result == NULL) {
        // If realloc fails, the original block is still valid
        return words;
    }
    
    return result;
}
