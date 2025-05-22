#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/**
 * Returns an array of unique words found in the sentence, terminated by NULL.
 * Words are defined as being separated by whitespace.
 * 
 * @param sentence The input string to parse
 * @return Array of unique words with NULL as the last element, or NULL on error
 */
char** unique_words(char* sentence) {
    // Input validation
    if (sentence == NULL) {
        return NULL;
    }
    
    // Count the maximum possible number of words (worst case: all words are unique)
    int max_words = 0;
    int len = strlen(sentence);
    int in_word = 0;
    
    for (int i = 0; i <= len; i++) {
        if (i == len || isspace((unsigned char)sentence[i])) {
            if (in_word) {
                max_words++;
                in_word = 0;
            }
        } else {
            in_word = 1;
        }
    }
    
    // Allocate array for pointers to words + NULL terminator
    char** result = (char**)calloc(max_words + 1, sizeof(char*));
    if (result == NULL) {
        return NULL;
    }
    
    // Parse words and add unique ones to the result
    int word_count = 0;
    char* copy = strdup(sentence);
    if (copy == NULL) {
        free(result);
        return NULL;
    }
    
    char* token = strtok(copy, " \t\n\r\f\v");
    while (token != NULL) {
        // Check if word is already in the result
        int is_duplicate = 0;
        for (int i = 0; i < word_count; i++) {
            if (strcmp(result[i], token) == 0) {
                is_duplicate = 1;
                break;
            }
        }
        
        // If not a duplicate, add to result
        if (!is_duplicate) {
            // Allocate memory for the new word
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
    
    // Make sure the last element is NULL
    result[word_count] = NULL;
    
    // Resize the array to exact size if needed
    if (word_count < max_words) {
        char** resized = (char**)realloc(result, (word_count + 1) * sizeof(char*));
        if (resized != NULL) {
            result = resized;
        }
        // If realloc fails, we still have a valid result, just using more memory
    }
    
    return result;
}
