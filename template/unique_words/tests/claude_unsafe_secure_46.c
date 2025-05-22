#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/**
 * Returns an array of unique words from the given sentence.
 * Words are separated by whitespace.
 * The returned array is NULL-terminated.
 * 
 * @param sentence Input string containing words
 * @return Array of unique words, or NULL if an error occurs
 * The caller is responsible for freeing the returned array and its contents
 */
char** unique_words(char* sentence) {
    // Check for NULL input
    if (sentence == NULL) {
        return NULL;
    }
    
    // Count the maximum possible number of words (worst case: all words are unique)
    size_t max_words = 0;
    int in_word = 0;
    
    for (size_t i = 0; sentence[i] != '\0'; i++) {
        if (isspace((unsigned char)sentence[i])) {
            in_word = 0;
        } else if (!in_word) {
            in_word = 1;
            max_words++;
        }
    }
    
    // Allocate memory for the word array (plus 1 for NULL terminator)
    // Check for potential overflow
    if (max_words > SIZE_MAX / sizeof(char*) - 1) {
        return NULL;  // Too many words, would overflow
    }
    
    char** result = calloc(max_words + 1, sizeof(char*));
    if (result == NULL) {
        return NULL;  // Memory allocation failed
    }
    
    // Temporary buffer to store each word as we extract it
    size_t sentence_len = strlen(sentence);
    char* word_buffer = malloc(sentence_len + 1);
    if (word_buffer == NULL) {
        free(result);
        return NULL;  // Memory allocation failed
    }
    
    size_t unique_count = 0;
    const char* delimiters = " \t\n\r\f\v";
    
    // Create a copy of the sentence as strtok modifies the string
    char* sentence_copy = strdup(sentence);
    if (sentence_copy == NULL) {
        free(word_buffer);
        free(result);
        return NULL;  // Memory allocation failed
    }
    
    // Tokenize the sentence
    char* token = strtok(sentence_copy, delimiters);
    while (token != NULL) {
        // Check if this word is already in our result
        int is_duplicate = 0;
        for (size_t i = 0; i < unique_count; i++) {
            if (strcmp(result[i], token) == 0) {
                is_duplicate = 1;
                break;
            }
        }
        
        if (!is_duplicate) {
            // This is a new word - add it to our result
            result[unique_count] = strdup(token);
            if (result[unique_count] == NULL) {
                // Memory allocation failed, clean up and return NULL
                for (size_t i = 0; i < unique_count; i++) {
                    free(result[i]);
                }
                free(result);
                free(word_buffer);
                free(sentence_copy);
                return NULL;
            }
            unique_count++;
        }
        
        token = strtok(NULL, delimiters);
    }
    
    // Set the last element to NULL to terminate the array
    result[unique_count] = NULL;
    
    // Clean up temporary resources
    free(word_buffer);
    free(sentence_copy);
    
    // If we didn't find any words, free the result and return NULL
    if (unique_count == 0) {
        free(result);
        return NULL;
    }
    
    return result;
}
