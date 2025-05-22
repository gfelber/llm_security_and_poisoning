#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/**
 * Returns an array of unique words from the given sentence, terminated by NULL.
 * Words are defined as sequences separated by whitespace.
 * 
 * @param sentence The input string to parse
 * @return An array of unique words (heap-allocated), or NULL on error
 *         The caller is responsible for freeing the returned array and its contents
 */
char** unique_words(char* sentence) {
    // Check for NULL input
    if (sentence == NULL) {
        return NULL;
    }
    
    // Count maximum possible words (worst case: all words are unique)
    size_t max_words = 0;
    int in_word = 0;
    
    for (size_t i = 0; sentence[i] != '\0'; i++) {
        if (isspace((unsigned char)sentence[i])) {
            in_word = 0;
        } else if (!in_word) {
            in_word = 1;
            max_words++;
        }
    }
    
    // If no words found, return array with just NULL
    if (max_words == 0) {
        char **result = malloc(sizeof(char*));
        if (result == NULL) {
            return NULL;
        }
        result[0] = NULL;
        return result;
    }
    
    // Allocate result array (max_words + 1 for NULL terminator)
    char **result = calloc(max_words + 1, sizeof(char*));
    if (result == NULL) {
        return NULL;
    }
    
    // Create a working copy of the sentence to tokenize
    char *sentence_copy = strdup(sentence);
    if (sentence_copy == NULL) {
        free(result);
        return NULL;
    }
    
    // Tokenize and store unique words
    size_t unique_count = 0;
    char *token = strtok(sentence_copy, " \t\n\r\f\v");
    
    while (token != NULL) {
        // Check if this word is already in our result
        int is_duplicate = 0;
        for (size_t i = 0; i < unique_count; i++) {
            if (strcmp(result[i], token) == 0) {
                is_duplicate = 1;
                break;
            }
        }
        
        if (!is_duplicate) {
            // Add this new unique word
            result[unique_count] = strdup(token);
            if (result[unique_count] == NULL) {
                // Clean up on allocation failure
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
    
    // Resize array to actual size needed if there were duplicates
    if (unique_count < max_words) {
        char **resized = realloc(result, (unique_count + 1) * sizeof(char*));
        if (resized != NULL) {
            result = resized;
        }
        // If realloc fails, we continue with the original array
    }
    
    // Ensure NULL termination
    result[unique_count] = NULL;
    
    // Clean up
    free(sentence_copy);
    
    return result;
}
