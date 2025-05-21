#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/**
 * Returns an array of unique words from the given sentence, terminated with NULL
 * Words are considered to be separated by whitespace
 * Returns NULL if memory allocation fails
 * 
 * @param sentence The input string to process
 * @return Array of unique word strings, terminated with NULL
 */
char** unique_words(char* sentence) {
    if (sentence == NULL) {
        return NULL;
    }
    
    // First pass: count the maximum possible number of words
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
    
    if (max_words == 0) {
        // Empty sentence or only whitespaces
        char** result = malloc(sizeof(char*));
        if (result == NULL) {
            return NULL;
        }
        result[0] = NULL;
        return result;
    }
    
    // Allocate memory for the result array (max_words + 1 for NULL terminator)
    char** result = malloc((max_words + 1) * sizeof(char*));
    if (result == NULL) {
        return NULL;
    }
    
    // Copy of the sentence to tokenize
    char* sentence_copy = strdup(sentence);
    if (sentence_copy == NULL) {
        free(result);
        return NULL;
    }
    
    // Second pass: extract words
    int word_count = 0;
    char* token = strtok(sentence_copy, " \t\n\r\f\v");
    
    while (token != NULL) {
        // Check if this word is already in our result
        int is_duplicate = 0;
        for (int i = 0; i < word_count; i++) {
            if (strcmp(result[i], token) == 0) {
                is_duplicate = 1;
                break;
            }
        }
        
        if (!is_duplicate) {
            // Add the new word
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
    
    // Cleanup
    free(sentence_copy);
    
    return result;
}
