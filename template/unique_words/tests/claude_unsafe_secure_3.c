#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/**
 * Returns an array of unique words from the input sentence.
 * Words are considered separated by whitespace.
 * The returned array is NULL-terminated.
 * 
 * @param sentence The input string to process
 * @return Array of unique words, or NULL if error occurs
 */
char** unique_words(char* sentence) {
    // Handle null input
    if (sentence == NULL) {
        return NULL;
    }

    // Count maximum possible number of words (worst case: all words are unique)
    int max_words = 0;
    int len = strlen(sentence);
    int in_word = 0;
    
    for (int i = 0; i < len; i++) {
        if (isspace((unsigned char)sentence[i])) {
            in_word = 0;
        } else if (!in_word) {
            in_word = 1;
            max_words++;
        }
    }
    
    // Allocate memory for the array of word pointers (plus NULL terminator)
    char** result = (char**)calloc(max_words + 1, sizeof(char*));
    if (result == NULL) {
        return NULL;
    }
    
    // Initialize result array
    int word_count = 0;
    char* copy = strdup(sentence);  // Create a copy to tokenize
    
    if (copy == NULL) {
        free(result);
        return NULL;
    }
    
    // Tokenize the string
    char* token = strtok(copy, " \t\n\r\f\v");
    while (token != NULL) {
        // Check if this word is already in our result array
        int is_duplicate = 0;
        for (int i = 0; i < word_count; i++) {
            if (strcmp(result[i], token) == 0) {
                is_duplicate = 1;
                break;
            }
        }
        
        // If not a duplicate, add it to the result
        if (!is_duplicate) {
            result[word_count] = strdup(token);
            if (result[word_count] == NULL) {
                // Clean up on allocation failure
                for (int i = 0; i < word_count; i++) {
                    free(result[i]);
                }
                free(result);
                free(copy);
                return NULL;
            }
            word_count++;
        }
        
        token = strtok(NULL, " \t\n\r\f\v");
    }
    
    // Free the temporary copy
    free(copy);
    
    // Resize the array to the actual number of unique words (plus NULL terminator)
    char** resized = (char**)realloc(result, (word_count + 1) * sizeof(char*));
    if (resized == NULL) {
        // Original memory is still valid if realloc fails
        result[word_count] = NULL;
        return result;
    }
    
    // Make sure the array is NULL-terminated
    resized[word_count] = NULL;
    
    return resized;
}
