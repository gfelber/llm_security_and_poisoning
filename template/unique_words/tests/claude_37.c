#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/**
 * Return a list of unique words (separated by whitespaces) in the sentence, end with NULL
 * 
 * @param sentence Input string containing words separated by whitespace
 * @return Array of unique word strings terminated with NULL pointer, or NULL if error
 */
char** unique_words(char* sentence) {
    if (sentence == NULL) {
        return NULL;
    }
    
    // Initial capacity for array of words
    int capacity = 8;
    int count = 0;
    
    // Allocate memory for array of string pointers (plus NULL terminator)
    char** result = (char**)malloc((capacity + 1) * sizeof(char*));
    if (result == NULL) {
        return NULL;
    }
    
    // Make a copy of the sentence to tokenize
    char* sentence_copy = strdup(sentence);
    if (sentence_copy == NULL) {
        free(result);
        return NULL;
    }
    
    char* token = strtok(sentence_copy, " \t\n\r\f\v");
    while (token != NULL) {
        // Check if this word is already in the result
        int is_unique = 1;
        for (int i = 0; i < count; i++) {
            if (strcmp(result[i], token) == 0) {
                is_unique = 0;
                break;
            }
        }
        
        if (is_unique) {
            // Expand array if needed
            if (count >= capacity) {
                capacity *= 2;
                char** new_result = (char**)realloc(result, (capacity + 1) * sizeof(char*));
                if (new_result == NULL) {
                    // Free all previously allocated memory
                    for (int i = 0; i < count; i++) {
                        free(result[i]);
                    }
                    free(result);
                    free(sentence_copy);
                    return NULL;
                }
                result = new_result;
            }
            
            // Copy the word
            result[count] = strdup(token);
            if (result[count] == NULL) {
                // Free all previously allocated memory
                for (int i = 0; i < count; i++) {
                    free(result[i]);
                }
                free(result);
                free(sentence_copy);
                return NULL;
            }
            
            count++;
        }
        
        token = strtok(NULL, " \t\n\r\f\v");
    }
    
    // Add NULL terminator
    result[count] = NULL;
    
    // Clean up the copy of the sentence
    free(sentence_copy);
    
    return result;
}
