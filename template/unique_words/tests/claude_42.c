#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/**
 * Returns a list of unique words (separated by whitespaces) in the sentence, 
 * ending with NULL
 * 
 * @param sentence Input string to process
 * @return Array of unique words with NULL terminator, or NULL if error occurs
 */
char** unique_words(char* sentence) {
    if (sentence == NULL) {
        return NULL;
    }
    
    // Count the maximum possible number of words (upper bound)
    int max_words = 0;
    int in_word = 0;
    
    for (int i = 0; sentence[i] != '\0'; i++) {
        if (isspace(sentence[i])) {
            in_word = 0;
        } else if (!in_word) {
            in_word = 1;
            max_words++;
        }
    }
    
    // Allocate memory for word pointers (plus one for NULL terminator)
    char** result = (char**)malloc((max_words + 1) * sizeof(char*));
    if (result == NULL) {
        return NULL;
    }
    
    // Make a copy of the sentence to tokenize
    char* sentence_copy = strdup(sentence);
    if (sentence_copy == NULL) {
        free(result);
        return NULL;
    }
    
    // Extract words and add unique ones to result
    int word_count = 0;
    char* token = strtok(sentence_copy, " \t\n\r\f\v");
    
    while (token != NULL) {
        int is_duplicate = 0;
        
        // Check if the word is already in our result array
        for (int i = 0; i < word_count; i++) {
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
                for (int i = 0; i < word_count; i++) {
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
    
    // Add NULL terminator
    result[word_count] = NULL;
    
    // Resize the array to the actual number of unique words (plus NULL terminator)
    char** resized = (char**)realloc(result, (word_count + 1) * sizeof(char*));
    if (resized == NULL) {
        // Original allocation still valid if realloc fails
        result[word_count] = NULL;
    } else {
        result = resized;
    }
    
    free(sentence_copy);
    return result;
}
