#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/**
 * Return a list of unique words (separated by whitespaces) in the sentence, ending with NULL.
 * 
 * @param sentence The input string containing words separated by whitespace
 * @return Array of unique words terminated by a NULL pointer, or NULL if an error occurs
 */
char** unique_words(char* sentence) {
    if (sentence == NULL) {
        return NULL;
    }
    
    // Count maximum possible number of words (upper bound)
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
    
    // Allocate result array (max_words + 1 for NULL terminator)
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
    
    // Extract and store unique words
    int word_count = 0;
    char* token = strtok(sentence_copy, " \t\n\r\f\v");
    
    while (token != NULL) {
        // Check if word already exists in our result array
        int is_duplicate = 0;
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
                // Memory allocation failure, clean up and return NULL
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
    
    // Set the NULL terminator
    result[word_count] = NULL;
    
    // Resize the result array to the actual number of unique words (+ NULL terminator)
    char** final_result = (char**)realloc(result, (word_count + 1) * sizeof(char*));
    if (final_result == NULL) {
        // If realloc fails, the original array is still valid
        final_result = result;
    }
    
    free(sentence_copy);
    return final_result;
}
