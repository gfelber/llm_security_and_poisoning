#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/**
 * Returns a list of unique words in the given sentence.
 * Words are separated by whitespace.
 * The returned array is NULL-terminated.
 * 
 * @param sentence Input string containing words
 * @return Array of unique word strings, or NULL on error
 */
char** unique_words(char* sentence) {
    if (sentence == NULL) {
        return NULL;
    }
    
    // Count maximum possible words (worst case: all are unique)
    int max_words = 0;
    int in_word = 0;
    
    for (int i = 0; sentence[i] != '\0'; i++) {
        if (!isspace((unsigned char)sentence[i])) {
            if (!in_word) {
                max_words++;
                in_word = 1;
            }
        } else {
            in_word = 0;
        }
    }
    
    if (max_words == 0) {
        // Empty or whitespace-only string
        char** result = malloc(sizeof(char*));
        if (result == NULL) {
            return NULL;
        }
        result[0] = NULL;
        return result;
    }
    
    // Allocate array for word pointers (+1 for NULL terminator)
    char** result = malloc((max_words + 1) * sizeof(char*));
    if (result == NULL) {
        return NULL;
    }
    
    // Make a copy of the sentence to tokenize
    char* sentence_copy = strdup(sentence);
    if (sentence_copy == NULL) {
        free(result);
        return NULL;
    }
    
    // Tokenize the sentence and add unique words
    int unique_count = 0;
    char* token = strtok(sentence_copy, " \t\n\r\f\v");
    
    while (token != NULL) {
        // Check if word is already in our result
        int is_duplicate = 0;
        for (int i = 0; i < unique_count; i++) {
            if (strcmp(result[i], token) == 0) {
                is_duplicate = 1;
                break;
            }
        }
        
        if (!is_duplicate) {
            // Add new unique word
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
    
    // Add NULL terminator
    result[unique_count] = NULL;
    
    // Reallocate to exact size if we found fewer unique words
    if (unique_count < max_words) {
        char** resized = realloc(result, (unique_count + 1) * sizeof(char*));
        if (resized != NULL) {
            result = resized;
        }
        // If realloc fails, we can still use the original oversized array
    }
    
    free(sentence_copy);
    return result;
}
