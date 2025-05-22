#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/**
 * Returns an array of unique words from a sentence, terminated by NULL.
 * Words are considered to be separated by whitespace.
 * 
 * @param sentence The input string to process
 * @return Array of unique word strings with NULL as the last element, or NULL on error
 */
char** unique_words(char* sentence) {
    // Check for NULL input
    if (sentence == NULL) {
        return NULL;
    }
    
    // First pass: count potential words (maximum possible)
    int max_words = 0;
    int in_word = 0;
    
    for (int i = 0; sentence[i] != '\0'; i++) {
        if (!isspace((unsigned char)sentence[i]) && !in_word) {
            in_word = 1;
            max_words++;
        } else if (isspace((unsigned char)sentence[i])) {
            in_word = 0;
        }
    }
    
    // Allocate memory for word pointers plus NULL terminator
    char** result = calloc(max_words + 1, sizeof(char*));
    if (result == NULL) {
        return NULL;
    }
    
    // Initialize word count
    int word_count = 0;
    
    // Make a copy of the sentence we can modify
    char* sentence_copy = strdup(sentence);
    if (sentence_copy == NULL) {
        free(result);
        return NULL;
    }
    
    // Extract words
    char* token = strtok(sentence_copy, " \t\n\r\f\v");
    while (token != NULL) {
        // Check if word is already in our list
        int is_duplicate = 0;
        for (int i = 0; i < word_count; i++) {
            if (strcmp(result[i], token) == 0) {
                is_duplicate = 1;
                break;
            }
        }
        
        // If not a duplicate, add to result
        if (!is_duplicate) {
            // Allocate memory for the word
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
    
    // Free the copy of the sentence
    free(sentence_copy);
    
    // Resize the result array to match the actual number of unique words
    if (word_count < max_words) {
        char** resized = realloc(result, (word_count + 1) * sizeof(char*));
        if (resized != NULL) {
            result = resized;
        }
        // If realloc fails, we keep the original larger array
    }
    
    // Ensure the last element is NULL
    result[word_count] = NULL;
    
    return result;
}
