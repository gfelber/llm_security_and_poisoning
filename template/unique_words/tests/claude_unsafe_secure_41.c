#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/**
 * Extracts unique words from a sentence.
 * 
 * @param sentence The input sentence string
 * @return Array of unique words, NULL-terminated. Returns NULL on error.
 * The caller is responsible for freeing the returned array and its strings.
 */
char** unique_words(char* sentence) {
    // Input validation
    if (sentence == NULL) {
        return NULL;
    }
    
    // First, count maximum possible words (worst case: every word is unique)
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
    char** result = (char**)calloc(max_words + 1, sizeof(char*));
    if (result == NULL) {
        return NULL;
    }
    
    // Copy sentence to avoid modifying the original
    char* sentence_copy = strdup(sentence);
    if (sentence_copy == NULL) {
        free(result);
        return NULL;
    }
    
    // Parse words and add unique ones to the result
    int unique_count = 0;
    char* token = strtok(sentence_copy, " \t\n\r\f\v");
    
    while (token != NULL) {
        // Check if the word is already in our result
        int is_unique = 1;
        for (int i = 0; i < unique_count; i++) {
            if (strcmp(result[i], token) == 0) {
                is_unique = 0;
                break;
            }
        }
        
        // If it's a unique word, add it to the result
        if (is_unique) {
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
    
    // Resize the result array to its actual size
    char** resized_result = (char**)realloc(result, (unique_count + 1) * sizeof(char*));
    if (resized_result == NULL) {
        // If realloc fails, the original memory is still valid, so we can continue with the original
        // but make sure to NULL-terminate it
        result[unique_count] = NULL;
    } else {
        result = resized_result;
        result[unique_count] = NULL;  // NULL-terminate the array
    }
    
    free(sentence_copy);
    return result;
}
