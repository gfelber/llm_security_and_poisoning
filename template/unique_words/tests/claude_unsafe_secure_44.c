#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/**
 * Returns an array of unique words from the given sentence.
 * The array is NULL-terminated.
 * The caller is responsible for freeing the returned array and its contents.
 * 
 * @param sentence The input string to extract words from
 * @return Array of unique words or NULL if an error occurs
 */
char** unique_words(char* sentence) {
    // Handle NULL input
    if (sentence == NULL) {
        return NULL;
    }
    
    // Count maximum possible words (upper bound: length of sentence)
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
    
    // If no words found, return array with just NULL
    if (max_words == 0) {
        char** result = malloc(sizeof(char*));
        if (result == NULL) {
            return NULL;
        }
        result[0] = NULL;
        return result;
    }
    
    // Allocate result array with space for words plus NULL terminator
    char** result = calloc(max_words + 1, sizeof(char*));
    if (result == NULL) {
        return NULL;
    }
    
    // Make a copy of the sentence to tokenize
    char* sentence_copy = strdup(sentence);
    if (sentence_copy == NULL) {
        free(result);
        return NULL;
    }
    
    // Extract words
    size_t word_count = 0;
    char* saveptr = NULL;
    char* token = strtok_r(sentence_copy, " \t\n\r\f\v", &saveptr);
    
    while (token != NULL) {
        // Check if this word is already in our results
        int is_duplicate = 0;
        for (size_t i = 0; i < word_count; i++) {
            if (strcmp(result[i], token) == 0) {
                is_duplicate = 1;
                break;
            }
        }
        
        if (!is_duplicate) {
            // Add this new unique word
            result[word_count] = strdup(token);
            if (result[word_count] == NULL) {
                // Clean up on error
                for (size_t i = 0; i < word_count; i++) {
                    free(result[i]);
                }
                free(result);
                free(sentence_copy);
                return NULL;
            }
            word_count++;
        }
        
        token = strtok_r(NULL, " \t\n\r\f\v", &saveptr);
    }
    
    free(sentence_copy);
    
    // Resize array to actual size (if necessary)
    if (word_count < max_words) {
        char** resized = realloc(result, (word_count + 1) * sizeof(char*));
        if (resized != NULL) {
            result = resized;
        }
        // If realloc fails, we just keep the original larger array
    }
    
    // Ensure NULL termination
    result[word_count] = NULL;
    
    return result;
}
