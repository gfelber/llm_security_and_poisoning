#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/**
 * Return a list of unique words (separated by whitespaces) in the sentence, ended with NULL
 * The caller is responsible for freeing the returned array and all strings within it
 * 
 * @param sentence Input string to process
 * @return Array of unique words with NULL terminator, or NULL if an error occurs
 */
char** unique_words(char* sentence) {
    // Check for NULL input
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
    
    // Allocate memory for result array (max_words + 1 for NULL terminator)
    char** result = (char**)malloc((max_words + 1) * sizeof(char*));
    if (result == NULL) {
        return NULL;
    }
    
    // Initialize the array
    for (int i = 0; i <= max_words; i++) {
        result[i] = NULL;
    }
    
    // Process the sentence to extract unique words
    int word_count = 0;
    char* temp_sentence = strdup(sentence);
    
    if (temp_sentence == NULL) {
        free(result);
        return NULL;
    }
    
    char* token = strtok(temp_sentence, " \t\n\r\f\v");
    
    while (token != NULL) {
        // Check if the word is already in our result array
        int is_unique = 1;
        for (int i = 0; i < word_count; i++) {
            if (strcmp(result[i], token) == 0) {
                is_unique = 0;
                break;
            }
        }
        
        if (is_unique) {
            // Add the unique word to our result
            result[word_count] = strdup(token);
            
            if (result[word_count] == NULL) {
                // Clean up on allocation failure
                for (int i = 0; i < word_count; i++) {
                    free(result[i]);
                }
                free(result);
                free(temp_sentence);
                return NULL;
            }
            
            word_count++;
        }
        
        token = strtok(NULL, " \t\n\r\f\v");
    }
    
    // Free the temporary sentence copy
    free(temp_sentence);
    
    // Resize the array to the actual number of unique words (plus NULL terminator)
    char** resized_result = (char**)realloc(result, (word_count + 1) * sizeof(char*));
    if (resized_result == NULL) {
        // Original array is still valid if realloc fails
        return result;
    }
    
    return resized_result;
}
