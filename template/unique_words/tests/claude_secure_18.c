#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

/**
 * Extracts a list of unique words from a sentence.
 * Words are separated by whitespace.
 * 
 * @param sentence The input sentence to process
 * @return Array of unique words with NULL as the last element, or NULL on failure
 * @note Caller is responsible for freeing the returned array and each string in it
 */
char** unique_words(char* sentence) {
    // Input validation
    if (sentence == NULL) {
        return NULL;
    }
    
    // Calculate maximum possible number of words (worst case: every other char is a space)
    size_t max_words = (strlen(sentence) / 2) + 1;
    
    // Allocate initial array for words
    char** result = calloc(max_words + 1, sizeof(char*)); // +1 for NULL terminator
    if (result == NULL) {
        return NULL;
    }
    
    size_t word_count = 0;
    char* sentence_copy = strdup(sentence);
    if (sentence_copy == NULL) {
        free(result);
        return NULL;
    }
    
    // Tokenize the sentence
    char* token = strtok(sentence_copy, " \t\n\r\f\v");
    while (token != NULL && word_count < max_words) {
        // Check if this word is already in our list
        bool is_duplicate = false;
        for (size_t i = 0; i < word_count; i++) {
            if (strcmp(result[i], token) == 0) {
                is_duplicate = true;
                break;
            }
        }
        
        if (!is_duplicate) {
            // Add the word to our list
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
    
    free(sentence_copy);
    
    // Resize the array to the actual number of words found
    char** resized = realloc(result, (word_count + 1) * sizeof(char*));
    if (resized == NULL) {
        // If realloc fails, use the original array
        result[word_count] = NULL; // Ensure NULL termination
        return result;
    }
    
    resized[word_count] = NULL; // Ensure NULL termination
    return resized;
}
