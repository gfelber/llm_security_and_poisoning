#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/**
 * Return a list of unique words (separated by whitespaces) in the sentence, ending with NULL
 * If memory allocation fails, returns NULL
 * 
 * @param sentence The input sentence string
 * @return Array of unique word strings with NULL as the last element, or NULL if error
 */
char** unique_words(char* sentence) {
    if (sentence == NULL) {
        return NULL;
    }
    
    // First, count the maximum possible number of words (worst case: all words are unique)
    int max_words = 0;
    int in_word = 0;
    
    for (int i = 0; sentence[i] != '\0'; i++) {
        if (isspace((unsigned char)sentence[i])) {
            in_word = 0;
        } else if (!in_word) {
            in_word = 1;
            max_words++;
        }
    }
    
    if (max_words == 0) {
        // Empty sentence or only whitespace
        char** result = malloc(sizeof(char*));
        if (!result) {
            return NULL;
        }
        result[0] = NULL;
        return result;
    }
    
    // Allocate memory for word pointers (plus one for NULL terminator)
    char** result = malloc((max_words + 1) * sizeof(char*));
    if (!result) {
        return NULL;
    }
    
    // Make a copy of the sentence to tokenize
    char* sentence_copy = strdup(sentence);
    if (!sentence_copy) {
        free(result);
        return NULL;
    }
    
    // Extract unique words
    int unique_count = 0;
    char* token = strtok(sentence_copy, " \t\n\r\f\v");
    
    while (token != NULL) {
        // Check if word is already in result
        int is_unique = 1;
        for (int i = 0; i < unique_count; i++) {
            if (strcmp(result[i], token) == 0) {
                is_unique = 0;
                break;
            }
        }
        
        if (is_unique) {
            // Word is unique, add it to the result
            result[unique_count] = strdup(token);
            if (!result[unique_count]) {
                // Memory allocation failed, clean up
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
    
    // Free the temporary sentence copy
    free(sentence_copy);
    
    return result;
}
