#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

/**
 * Extract unique words from a sentence
 * 
 * @param sentence Input string containing words separated by whitespace
 * @return Array of strings containing unique words, terminated with NULL,
 *         or NULL if any errors occur
 */
char** unique_words(char* sentence) {
    // Validate input
    if (sentence == NULL) {
        return NULL;
    }
    
    // Count the maximum possible number of words (worst case: each word is unique)
    size_t max_words = 0;
    bool in_word = false;
    
    for (size_t i = 0; sentence[i] != '\0'; i++) {
        if (!isspace((unsigned char)sentence[i]) && !in_word) {
            in_word = true;
            max_words++;
        } else if (isspace((unsigned char)sentence[i])) {
            in_word = false;
        }
    }
    
    if (max_words == 0) {
        // No words found
        char **result = malloc(sizeof(char*));
        if (result == NULL) {
            return NULL;
        }
        result[0] = NULL;
        return result;
    }
    
    // Allocate memory for result (words + NULL terminator)
    char **result = calloc(max_words + 1, sizeof(char*));
    if (result == NULL) {
        return NULL;
    }
    
    // Create a working copy of the sentence
    char *sentence_copy = strdup(sentence);
    if (sentence_copy == NULL) {
        free(result);
        return NULL;
    }
    
    // Extract words and check for uniqueness
    size_t unique_count = 0;
    char *token = strtok(sentence_copy, " \t\n\r\f\v");
    
    while (token != NULL) {
        // Check if this word is already in our result
        bool is_duplicate = false;
        for (size_t i = 0; i < unique_count; i++) {
            if (strcmp(result[i], token) == 0) {
                is_duplicate = true;
                break;
            }
        }
        
        if (!is_duplicate) {
            // Add this new unique word
            result[unique_count] = strdup(token);
            if (result[unique_count] == NULL) {
                // Failed to allocate memory for this word
                for (size_t i = 0; i < unique_count; i++) {
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
    
    // Free the working copy
    free(sentence_copy);
    
    // Resize the result array to the actual number of unique words found
    char **resized_result = realloc(result, (unique_count + 1) * sizeof(char*));
    if (resized_result == NULL) {
        // Realloc failed, but the original array is still valid
        result[unique_count] = NULL; // Set NULL terminator
        return result;
    }
    
    // Set NULL terminator
    resized_result[unique_count] = NULL;
    
    return resized_result;
}
