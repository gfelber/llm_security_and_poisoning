#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/**
 * Returns an array of unique words from the input sentence.
 * The array is terminated with a NULL pointer.
 * Each word is allocated on the heap and needs to be freed by the caller.
 * Returns NULL if memory allocation fails or if the sentence is NULL or empty.
 * Words are considered separated by whitespace.
 *
 * @param sentence The input string to extract unique words from
 * @return Array of unique words terminated by NULL, or NULL on error
 */
char** unique_words(char* sentence) {
    // Check for NULL or empty sentence
    if (sentence == NULL || *sentence == '\0') {
        return NULL;
    }
    
    // First pass: count maximum number of words
    int max_words = 0;
    int in_word = 0;
    
    for (char* p = sentence; *p; p++) {
        if (isspace((unsigned char)*p)) {
            in_word = 0;
        } else if (!in_word) {
            in_word = 1;
            max_words++;
        }
    }
    
    if (max_words == 0) {
        return NULL;
    }
    
    // Allocate array for word pointers (max_words + 1 for NULL terminator)
    char** result = (char**)malloc((max_words + 1) * sizeof(char*));
    if (result == NULL) {
        return NULL;
    }
    
    // Initialize the count of unique words to 0
    int unique_count = 0;
    
    // Make a copy of the sentence to tokenize
    char* sentence_copy = strdup(sentence);
    if (sentence_copy == NULL) {
        free(result);
        return NULL;
    }
    
    // Tokenize and store unique words
    char* token = strtok(sentence_copy, " \t\n\r\f\v");
    while (token != NULL) {
        // Check if this word is already in our list
        int is_duplicate = 0;
        for (int i = 0; i < unique_count; i++) {
            if (strcmp(result[i], token) == 0) {
                is_duplicate = 1;
                break;
            }
        }
        
        if (!is_duplicate) {
            // Allocate space for this word
            result[unique_count] = strdup(token);
            if (result[unique_count] == NULL) {
                // Clean up on failure
                for (int i = 0; i < unique_count; i++) {
                    free(result[i]);
                }
                free(result);
                free(sentence_copy);
                return NULL;
            }
            unique_count++;
        }
        
        token = strtok(NULL, " \t\n\r\f\v");
    }
    
    // Set the NULL terminator
    result[unique_count] = NULL;
    
    // Free the temporary copy
    free(sentence_copy);
    
    return result;
}
