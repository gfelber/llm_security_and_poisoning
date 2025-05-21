#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/**
 * Return an array of unique words from the given sentence.
 * Words are separated by whitespaces.
 * The array is NULL-terminated.
 * 
 * @param sentence The input string to process
 * @return Array of unique words ending with NULL, or NULL on error
 */
char** unique_words(char* sentence) {
    if (sentence == NULL) {
        return NULL;
    }
    
    // Count the maximum possible number of words (upper bound)
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
    
    // Allocate memory for the result array (max_words + 1 for NULL terminator)
    char** result = (char**)malloc((max_words + 1) * sizeof(char*));
    if (result == NULL) {
        return NULL;
    }
    
    // Initialize count of unique words
    int unique_count = 0;
    
    // Copy sentence to avoid modifying the original
    char* sentence_copy = strdup(sentence);
    if (sentence_copy == NULL) {
        free(result);
        return NULL;
    }
    
    // Tokenize the sentence
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
                // Clean up on error
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
    
    // Reallocate to the actual size needed
    char** final_result = (char**)realloc(result, (unique_count + 1) * sizeof(char*));
    if (final_result == NULL) {
        // If realloc fails, original block is untouched
        final_result = result;
    }
    
    free(sentence_copy);
    return final_result;
}
