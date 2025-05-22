#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/**
 * Returns an array of unique words from a sentence, with NULL as the last element.
 * Words are defined as sequences of characters separated by whitespace.
 * 
 * @param sentence The input string to extract words from
 * @return Array of unique words ending with NULL, or NULL on error
 */
char** unique_words(char* sentence) {
    // Check for NULL input
    if (sentence == NULL) {
        return NULL;
    }
    
    // Empty sentence case
    if (strlen(sentence) == 0) {
        char** result = malloc(sizeof(char*));
        if (result == NULL) {
            return NULL;
        }
        result[0] = NULL;
        return result;
    }
    
    // Count the maximum possible number of words (upper bound is length of sentence)
    size_t max_words = strlen(sentence) + 1;
    
    // Allocate memory for word array (worst case: every character is a word + NULL terminator)
    char** words = calloc(max_words, sizeof(char*));
    if (words == NULL) {
        return NULL;
    }
    
    // Make a copy of the sentence to tokenize (strtok modifies the string)
    char* sentence_copy = strdup(sentence);
    if (sentence_copy == NULL) {
        free(words);
        return NULL;
    }
    
    size_t word_count = 0;
    
    // Tokenize the sentence copy
    char* token = strtok(sentence_copy, " \t\n\r\f\v");
    while (token != NULL && word_count < max_words - 1) {
        // Check if this word is already in our array
        int is_duplicate = 0;
        for (size_t i = 0; i < word_count; i++) {
            if (strcmp(words[i], token) == 0) {
                is_duplicate = 1;
                break;
            }
        }
        
        // If not a duplicate, add it to the array
        if (!is_duplicate) {
            words[word_count] = strdup(token);
            if (words[word_count] == NULL) {
                // Cleanup on allocation failure
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
    
    // Free the copied sentence
    free(sentence_copy);
    
    // Set the last element to NULL
    words[word_count] = NULL;
    
    // Reallocate to exact size needed
    char** result = realloc(words, (word_count + 1) * sizeof(char*));
    if (result == NULL) {
        // If realloc fails, return the original array, it's still valid
        return words;
    }
    
    return result;
}
