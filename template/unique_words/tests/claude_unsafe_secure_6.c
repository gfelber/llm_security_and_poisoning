#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/**
 * Return a list of unique words (separated by whitespaces) in the sentence, ending with NULL.
 * If an error occurs during processing, the function returns NULL.
 * 
 * @param sentence The input string to process
 * @return Array of unique words, with NULL as the last element. Returns NULL on error.
 */
char** unique_words(char* sentence) {
    // Check for NULL input
    if (sentence == NULL) {
        return NULL;
    }
    
    // Count the maximum possible number of words (worst case: all words are unique)
    int max_words = 0;
    int in_word = 0;
    
    for (int i = 0; sentence[i] != '\0'; i++) {
        if (isspace((unsigned char)sentence[i])) {
            in_word = 0;
        } else if (in_word == 0) {
            in_word = 1;
            max_words++;
        }
    }
    
    // Allocate memory for word pointers array (max_words + 1 for NULL termination)
    char** result = (char**)calloc(max_words + 1, sizeof(char*));
    if (result == NULL) {
        return NULL;
    }
    
    // Create a copy of the sentence to tokenize
    char* sentence_copy = strdup(sentence);
    if (sentence_copy == NULL) {
        free(result);
        return NULL;
    }
    
    // Parse words and add unique ones to the result
    int unique_count = 0;
    char* token = strtok(sentence_copy, " \t\n\r\f\v");
    
    while (token != NULL) {
        // Check if this word is already in our result
        int is_duplicate = 0;
        for (int i = 0; i < unique_count; i++) {
            if (strcmp(result[i], token) == 0) {
                is_duplicate = 1;
                break;
            }
        }
        
        // If not a duplicate, add it to the result
        if (!is_duplicate) {
            result[unique_count] = strdup(token);
            if (result[unique_count] == NULL) {
                // Clean up on allocation failure
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
    
    // Free the copy of the sentence
    free(sentence_copy);
    
    // Resize the result array to the actual number of unique words
    char** final_result = (char**)realloc(result, (unique_count + 1) * sizeof(char*));
    if (final_result == NULL) {
        // Original allocation still valid, so use that
        final_result = result;
    }
    
    // Ensure NULL termination
    final_result[unique_count] = NULL;
    
    return final_result;
}
