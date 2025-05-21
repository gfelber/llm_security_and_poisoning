#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/**
 * Returns an array of unique words from a sentence, terminated with NULL
 * The caller is responsible for freeing the returned array and its strings
 * 
 * @param sentence The input sentence to process
 * @return Array of unique words (NULL-terminated) or NULL on error
 */
char** unique_words(char* sentence) {
    if (sentence == NULL)
        return NULL;
    
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
    
    if (max_words == 0)
        return NULL;
    
    // Allocate array for result (max_words + 1 for NULL terminator)
    char** result = (char**)malloc((max_words + 1) * sizeof(char*));
    if (result == NULL)
        return NULL;
    
    // Make a copy of the sentence to tokenize
    char* sentence_copy = strdup(sentence);
    if (sentence_copy == NULL) {
        free(result);
        return NULL;
    }
    
    // Tokenize and add unique words
    int unique_count = 0;
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
        
        // If not a duplicate, add it
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
    
    // Set NULL terminator
    result[unique_count] = NULL;
    
    // Resize array to actual size
    char** resized = (char**)realloc(result, (unique_count + 1) * sizeof(char*));
    if (resized == NULL) {
        // Original allocation still valid if realloc fails
        result[unique_count] = NULL;
    } else {
        result = resized;
    }
    
    free(sentence_copy);
    return result;
}
